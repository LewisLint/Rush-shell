" Use jj to exit insert mode
imap jj <Esc>

" Use ; for : in normal mode
nmap ; :

" Basic settings
set number
set autoindent
set tabstop=4
set shiftwidth=4

" --- CUSTOM SHORTCUT MAPPINGS ---
let mapleader = " "

" Map jk or jj to Escape (from your previous setup)
inoremap jj <Esc>
" Map semicolon to colon for faster commands (from your previous setup)
nnoremap ; :

" Instant C compilation and run with F5
nnoremap <F5> :w <bar> !gcc % -o %< <CR>

" Clear search highlights with Ctrl+L
nnoremap <C-l> :nohlsearch<CR>

" Open Left Explorer (Type a width number or just hit Enter to toggle close)
nnoremap <Leader>f :Lex 


" --- SYSTEM & VISUAL TWEAKS ---
set clipboard=unnamedplus   " Link Vim directly to system copy/paste
set mouse=a                 " Enable mouse support for scrolling/clicking
set hlsearch                " Highlight all search matches
set incsearch               " Highlight matches dynamically as you type


" --- CODE FORMATTING ---
set autoindent              " Maintain indentation from previous line
set tabstop=4               " Visual length of a tab character
set shiftwidth=4            " Spaces used for auto-indent steps
set expandtab               " Convert tabs to spaces for clean GitHub views


" --- NATIVE FILE EXPLORER (NETRW) ---
let g:netrw_banner = 0       " Hide the massive top help banner
let g:netrw_liststyle = 3    " Clean tree structure directory view
let g:netrw_browse_split = 4 " Open chosen files in a new vertical split


inoremap <M-h> <Left>
inoremap <M-j> <Down>
inoremap <M-k> <Up>
inoremap <M-l> <Right>
