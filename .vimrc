colorscheme monokai
syntax enable
let mapleader = ","

set autoindent
set tabstop=4
set shiftwidth=4
set softtabstop=4
set expandtab
set cursorline
set ruler

set number
filetype indent on
set wildmenu

set incsearch
set hlsearch
nnoremap <leader><space> :nohlsearch<CR>

hi clear CursorLine
augroup CLClear
    autocmd! ColorScheme * hi clear CursorLine
augroup END
hi CursorLineNR cterm=bold
augroup CLNRSet
    autocmd! ColorScheme * hi CursorLineNR cterm=bold
augroup END

autocmd filetype cpp nnoremap <F8> :w <bar> !clear && g++ -std=gnu++14 -O2 % -o %:p:h/%:t:r.exe && ./%:r.exe<CR>
autocmd filetype c nnoremap <F8> :w <bar> !clear && gcc -std=c99 -Wall -Werror -Wextra -lm % -o %:p:h/%:t:r.out && ./%:r.out<CR>
autocmd filetype java nnoremap <F8> :w <bar> !javac % && java -enableassertions %:p <CR>
autocmd filetype python nnoremap <F8> :w <bar> !python3 % <CR>
autocmd filetype perl nnoremap <F8> :w <bar> !perl % <CR>
autocmd filetype go nnoremap <F8> :w <bar> !go build % && ./%:p <CR>
autocmd filetype js nnoremap <F8> :w <bar> !node % <CR>

nmap <F2> :.w !pbcopy <CR><CR>
vmap <F2> :<CR>:let @a=@" \| execute "normal! vgvy" \| let res=system("pbcopy", @") \| let @"=@a<CR>

noremap <Up> <Nop>
noremap <Down> <Nop>
noremap <Left> <Nop>
noremap <Right> <Nop>

inoremap <Up> <Nop>
inoremap <Down> <Nop>
inoremap <Left> <Nop>
inoremap <Right> <Nop>
