" --- GENERAL SETTINGS ---
set number                  " Show line numbers
set mouse=a                 " Enable mouse support for scrolling/clicking
set clipboard=unnamedplus   " Link Vim directly to system copy/paste

" --- SEARCH TWEAKS ---
set hlsearch                " Highlight all search matches
set incsearch               " Highlight matches dynamically as you type

" --- CODE FORMATTING ---
set autoindent              " Maintain indentation from previous bullshit
set tabstop=4               " Length of a tab
set shiftwidth=4            " Spaces used for auto-indent steps
set expandtab               " Convert tabs to spaces

" --- CUSTOM SHORTCUT MAPPINGS ---
let mapleader = " " 

" Fast Escape and Command mode
inoremap jj <Esc> " click jj fast to go  to normal mode
nnoremap ; : "  click ; for : so no space because i dont wanna waste 3 fucking nanoseconds of my time

" Instant C compilation and run with F5
nnoremap <F5> :w <bar> !gcc % -o %:r && ./%:r<CR> " just click f5 to compile your c file  --   it prolly wont compile, you're shit at coding

" Clear search highlights with Ctrl+L
nnoremap <C-l> :nohlsearch<CR> "  the highlights just stay there for fucking forever

" Open Left Explorer (Tree view toggling)
nnoremap <Leader>f :Lex 30<CR> " this one is goddam useful, 

" --- INSERT MODE NAVIGATION & EDITING (Alt/Meta Keys) ---
inoremap <M-h> <Left>
inoremap <M-j> <Down>
inoremap <M-k> <Up>
inoremap <M-l> <Right>

" Word-by-word movement
inoremap <M-w> <C-o>w
inoremap <M-b> <C-o>b
inoremap <M-e> <C-o>e

" Efficient text deletion in Insert mode
inoremap <M-d> <C-o>dw
inoremap <M-x> <Delete>
inoremap <M-c> <C-o>diw

" --- NATIVE FILE EXPLORER (NETRW) ---
let g:netrw_banner = 0       " Hide the massive top help banner
let g:netrw_liststyle = 3    " Clean tree structure directory view
let g:netrw_browse_split = 4 " Open chosen files in a new vertical split
let g:netrw_winsize = 25     " Fix explorer window width to 25%
nnoremap <Leader><Space> /


" THEMES AND OTHER AESTHETIC SHIT

set background=dark " sets the background to dark, if you cant fucking read
colorscheme slate " idk  man this is supposed to be easy on the eyes for all you chronic terminal adicts.
set wildmenu "  figure it the fuck out
