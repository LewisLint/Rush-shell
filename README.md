# Rush

Rush is a small Unix shell written in C. It is a learning project for
processes, system calls, parsing, and terminal input.

## Features

- Run external commands with `fork()` and `execvp()`
- Directory-aware prompt
- `cd` and `exit` built-ins
- Double- and single-quoted arguments
- Basic command substitution with `$(command)`
- Script files
- GNU Readline history
- Vi editing mode (`Esc`, then `k`/`j` for history)

## Requirements

- Linux or another Unix-like operating system
- GCC or another C compiler
- GNU Readline development files

On Ubuntu, install Readline with:

```bash
sudo apt install libreadline-dev
```

## Build

From the `Rush` directory:

```bash
gcc -Wall -Wextra -std=c99 -o Rush rush.c -lreadline
```

For a smaller release binary:

```bash
gcc -Os -s -flto -std=c99 -o Rush rush.c -lreadline
```

## Run

Start the interactive shell:

```bash
./Rush
```

Run a script:

```bash
./Rush script.rush
```

Example script:

```text
pwd
echo "hello world"
echo $(ls)
cd ..
pwd
exit
```

## Install As `rush`

Create a personal executable directory and link the binary into it:

```bash
mkdir -p ~/.local/bin
ln -sf "$PWD/Rush" ~/.local/bin/rush
export PATH="$HOME/.local/bin:$PATH"
```

To keep that `PATH` change after restarting your terminal, add it to
`~/.bashrc`.

## Limitations

Rush is intentionally small and does not yet implement pipelines, redirection,
background jobs, environment-variable expansion, or a configuration file.




