" --- GENERAL SETTINGS ---
set number                  " Show line numbers
set mouse=a                 " Enable mouse support for scrolling/clicking
set clipboard=unnamedplus   " Link Vim directly to system copy/paste

" --- SEARCH TWEAKS ---
set hlsearch                " Highlight all search matches
set incsearch               " Highlight matches dynamically as you type

" --- CODE FORMATTING ---
set autoindent              " Maintain indentation from previous line
set tabstop=4               " Visual length of a tab character
set shiftwidth=4            " Spaces used for auto-indent steps
set expandtab               " Convert tabs to spaces

" --- CUSTOM SHORTCUT MAPPINGS ---
let mapleader = " "

" Fast Escape and Command mode
inoremap jj <Esc>
nnoremap ; :

" Instant C compilation and run with F5
nnoremap <F5> :w <bar> !gcc % -o %< && ./%<<CR>

" Clear search highlights with Ctrl+L
nnoremap <C-l> :nohlsearch<CR>

" Open Left Explorer (Tree view toggling)
nnoremap <Leader>f :Lex 30<CR>

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
