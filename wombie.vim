
hi clear
if exists("syntax_on")
	syntax reset
endif
set background=dark
let colors_name = "wombie"


hi Normal					ctermfg=252		ctermbg=none	cterm=none
hi Visual					ctermfg=none	ctermbg=237		cterm=none
hi Search					ctermfg=none	ctermbg=239		cterm=bold
hi Folded					ctermfg=27		ctermbg=none	cterm=none
hi Title 					ctermfg=84		ctermbg=none	cterm=bold
hi Status					ctermfg=230		ctermbg=none	cterm=none
hi VertSp					ctermfg=238		ctermbg=none	cterm=none
hi Status					ctermfg=241		ctermbg=none	cterm=none
hi StatusLine				ctermfg=241		ctermbg=0		cterm=none
hi LineNr					ctermfg=232		ctermbg=none	cterm=none
hi Special					ctermfg=241		ctermbg=none	cterm=none
hi WarningMsg				ctermfg=214
hi ErrorMsg					ctermfg=124		ctermbg=none	cterm=none
hi ColorColumn									ctermbg=234
hi ExtraWhitespace							ctermbg=001

hi Cursor					ctermfg=234		ctermbg=none	cterm=none
hi CursorIM					ctermbg=232							cterm=none
hi CursorLine				ctermbg=234							cterm=none
hi CursorLineNR			ctermfg=242		ctermbg=none	cterm=none
hi MatchParen				ctermfg=123		ctermbg=none	cterm=bold
hi Pmenu						ctermfg=230		ctermbg=none
hi PmenuSel					ctermfg=232		ctermbg=none

hi DiffAdd										ctermbg=none
hi DiffDelete				ctermfg=234		ctermbg=none	cterm=none
hi DiffText					ctermbg=none	cterm=none
hi DiffChange				ctermbg=none

" Syntax highlighting
hi Comment					ctermfg=240		ctermbg=none	cterm=none
hi Constant					ctermfg=084		ctermbg=none	cterm=none
hi Identifier				ctermfg=228		ctermbg=none	cterm=none
hi Statement				ctermfg=075		ctermbg=none	cterm=none
hi PreProc					ctermfg=141		ctermbg=none	cterm=none
hi Type						ctermfg=186		ctermbg=none	cterm=none
hi Special					ctermfg=230		ctermbg=none	cterm=none
" Underlined
" Ignore
" Error
hi Todo						ctermfg=203		ctermbg=none 	cterm=bold

"hi Number					ctermfg=078		ctermbg=none	cterm=none
"hi Function					ctermfg=192		ctermbg=none	cterm=none
"hi String					ctermfg=216		ctermbg=none	cterm=none


" Links
hi! link FoldColumn		Folded
hi! link CursorColumn	CursorLine
hi! link NonText			LineNr

hi SpellBad 				cterm=underline ctermfg=210 ctermbg=none
hi SpellCap 				cterm=underline ctermfg=13 ctermbg=none
hi SpellRare 				cterm=underline ctermfg=14 ctermbg=none
hi SpellLocal 				cterm=underline ctermfg=10 ctermbg=none

hi User1 					ctermbg=4 ctermfg=232
hi User2 					ctermbg=14 ctermfg=17 cterm=bold
hi User3 					ctermbg=234 ctermfg=240
hi User4 					ctermbg=234 ctermfg=10 cterm=bold
hi User5 					ctermbg=234 ctermfg=9 cterm=bold
hi User6 					ctermbg=14 ctermfg=4
hi User7 					ctermbg=0 ctermfg=14
hi User8 					ctermbg=0 ctermfg=234
hi User9 					ctermbg=234 ctermfg=14

" whitespace highlighting
match ExtraWhitespace /\s\+$/
autocmd InsertEnter * match ExtraWhitespace /\s\+\%#\@!$/
autocmd InsertLeave * match ExtraWhitespace /\s\+$/

