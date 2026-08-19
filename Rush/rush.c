#define _GNU_SOURCE

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/history.h>
#include <readline/readline.h>

static int expand_command_substitution(char **line)
{
    char *start = strstr(*line, "$(");

    if (start == NULL)
        return 0;

    char *end = strchr(start + 2, ')');

    if (end == NULL) {
        fprintf(stderr, "rush: unmatched command substitution\n");
        return -1;
    }

    size_t command_length = (size_t)(end - (start + 2));
    char *command = malloc(command_length + 1);

    if (command == NULL)
        return -1;

    memcpy(command, start + 2, command_length);
    command[command_length] = '\0';

    FILE *command_output = popen(command, "r");
    free(command);

    if (command_output == NULL) {
        perror("popen");
        return -1;
    }

    size_t output_length = 0;
    size_t output_capacity = 128;
    char *output = malloc(output_capacity);

    if (output == NULL) {
        pclose(command_output);
        return -1;
    }

    while (fgets(output + output_length,
                 (int)(output_capacity - output_length), command_output) != NULL) {
        output_length = strlen(output);

        if (output_capacity - output_length < 2) {
            output_capacity *= 2;
            char *expanded_output = realloc(output, output_capacity);

            if (expanded_output == NULL) {
                free(output);
                pclose(command_output);
                return -1;
            }

            output = expanded_output;
        }
    }

    pclose(command_output);

    while (output_length > 0 && output[output_length - 1] == '\n')
        output[--output_length] = '\0';

    size_t prefix_length = (size_t)(start - *line);
    size_t suffix_length = strlen(end + 1);
    char *expanded_line = malloc(prefix_length + output_length + suffix_length + 1);

    if (expanded_line == NULL) {
        free(output);
        return -1;
    }

    memcpy(expanded_line, *line, prefix_length);
    memcpy(expanded_line + prefix_length, output, output_length);
    memcpy(expanded_line + prefix_length + output_length, end + 1, suffix_length + 1);

    free(output);
    free(*line);
    *line = expanded_line;

    return 1;
}

static int split_arguments(char *line, char *arguments[], int limit)
{
    int count = 0;
    char *cursor = line;

    while (*cursor != '\0' && count < limit - 1) {
        while (*cursor == ' ' || *cursor == '\t')
            cursor++;

        if (*cursor == '\0')
            break;

        char *start = cursor;
        char quote = '\0';

        if (*cursor == '"' || *cursor == '\'')
            quote = *cursor++;

        start = cursor;
        arguments[count++] = start;

        if (quote != '\0') {
            while (*cursor != quote && *cursor != '\0')
                cursor++;

            if (*cursor == '\0') {
                fprintf(stderr, "rush: unmatched quote\n");
                return -1;
            }
        } else {
            while (*cursor != ' ' && *cursor != '\t' && *cursor != '\0')
                cursor++;
        }

        if (*cursor != '\0')
            *cursor++ = '\0';
    }

    arguments[count] = NULL;
    return count;
}

int main(int argc, char *argv[])
{
    char *line = NULL;
    size_t capacity = 0;
    FILE *input = stdin;

    if (argc > 2) {
        fprintf(stderr, "usage: %s [script]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            perror(argv[1]);
            return EXIT_FAILURE;
        }
    }

    int interactive = input == stdin && isatty(STDIN_FILENO);

    if (interactive) {
        char editing_mode[] = "set editing-mode vi";
        rl_parse_and_bind(editing_mode);
    }

    while (1) {
        char directory[PATH_MAX];
        char prompt[PATH_MAX + 4];

        if (interactive) {
            if (getcwd(directory, sizeof(directory)) != NULL)
                snprintf(prompt, sizeof(prompt), "%s > ", directory);
            else
                strcpy(prompt, "rush > ");

            line = readline(prompt);
        } else {
            if (getline(&line, &capacity, input) == -1)
                break;

            line[strcspn(line, "\n")] = '\0';
        }

        if (line == NULL)
            break;

        if (interactive && line[0] != '\0')
            add_history(line);

        int expanded = expand_command_substitution(&line);

        if (expanded == -1)
            goto next_line;

        if (!interactive && expanded == 1)
            capacity = 0;

        char *arguments[64];
        int count = split_arguments(line, arguments, 64);

        if (count < 0)
            goto next_line;

        if (count == 0)
            goto next_line;

        if (strcmp(arguments[0], "exit") == 0)
            break;

        if (strcmp(arguments[0], "cd") == 0) {
            if (arguments[1] == NULL)
                fprintf(stderr, "cd: missing directory\n");
            else if (chdir(arguments[1]) == -1)
                perror("cd");

            goto next_line;
        }

        pid_t child = fork();

        if (child == 0) {
            execvp(arguments[0], arguments);
            perror("execvp");
            exit(EXIT_FAILURE);
        }

        if (child > 0) {
            waitpid(child, NULL, 0);
        } else {
            perror("fork");
        }

next_line:
        if (interactive) {
            free(line);
            line = NULL;
        } else if (expanded == 1) {
            free(line);
            line = NULL;
            capacity = 0;
        }
    }

    free(line);

    if (interactive)
        clear_history();

    if (input != stdin)
        fclose(input);

    return 0;
}
