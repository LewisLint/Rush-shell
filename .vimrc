" --- GENERAL SETTINGS ---
set number                  " Show line numbers
set mouse=a                 " Enable mouse support for scrolling/clicking
set clipboard=unnamedplus   " Link Vim directly to system copy/paste

" Reduce wait time for Alt/Meta key sequences to register
set ttimeoutlen=50          

" --- SEARCH TWEAKS ---
set hlsearch                " Highlight all search matches
set incsearch               " Highlight matches dynamically as you type

" --- CODE FORMATTING ---
" Maintain indentation from previous line
set autoindent              
set tabstop=4               " Length of a tab
set shiftwidth=4            " Spaces used for auto-indent steps
set expandtab               " Convert tabs to spaces

" --- CUSTOM SHORTCUT MAPPINGS ---
let mapleader = " " 

" Use jj to return to normal mode
inoremap jj <Esc>

" Use ; to enter command mode quickly
nnoremap ; :

" Compile and run C file with F5
nnoremap <F5> :w<CR>:!gcc "%" -o "%:p:r" && "%:p:r"<CR>

" Clear search highlights with Ctrl+L
nnoremap <C-l> :nohlsearch<CR>

" Open/Toggle file explorer
nnoremap <Leader>f :Lex<CR>

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
let g:netrw_banner = 0       " Hide the top help banner
let g:netrw_liststyle = 3    " Clean tree structure directory view
let g:netrw_browse_split = 4 " Open chosen files in a new vertical split
let g:netrw_winsize = 25     " Fix explorer window width to 25%
nnoremap <Leader><Space> /

" --- THEMES AND AESTHETICS ---

" Use a dark background
set background=dark

" Set colorscheme
colorscheme slate

" Enhanced command line completion
set wildmenu
