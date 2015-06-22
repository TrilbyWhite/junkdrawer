#!/bin/bash
#############################################################################
#% *LOBSTER*
#% Copyright 2015, Jesse McClure
#% License CC-BY-SA
#%
#%    *Description*
#%      lobster will run a selected action on any number of entries in a bibfile.
#%
#%    *Usage*
#%      lobster *[* _options_ *]* *[* _keys_ *]*
#%
#############################################################################

# NOTE: lines begining with #% are a form of 'literate programming'
#       try `lobster -h` to see the formatted output

#%    *Variables*
#%      Default setting for the _bib_ _file_ can be adjusted by editing this script.
#%
bibfile=~/.refs.bib
bibeditor=vim
post_clean=1
rscfile=/tmp/bibedit.rsc
tmpfile=/tmp/bibedit.bib
pdfreader=mupdf
pdfpath=~/doc/pdfs/
browser=firefox
viewpipe="highlight -O xterm256 -S bib -s xoria256"

function main() {
	[[ $# -eq 0 ]] && $bibeditor $bibfile && exit
	cmd=view
	keys=""
	while getopts ":ab:cdef:hiklopr:s:v-" opt; do
#%    *Options*
		case $opt in
#%      -a
#%          select all entries
			a) keys="$(echo $(sed -e '/^[^@]/ d' -e 's/^@[^{]*{\([^,]*\),/\1/' $bibfile))" ;;
#%      -b _file_
#%          select bib file
			b) bibfile="$OPTARG" ;;
#%      -c
#%          disable bibtool cleaning
			c) post_clean=0 ;;
#%      -d
#%          delete selected entries
			d) cmd=cut ;;
#%      -e
#%          edit selected entries
			e) cmd=edit ;;
#%      -f _field_
#%          print field specified (title, author, etc)
#%          multiple fields must be separated by a + (e.g., _title+author_)
			f) cmd=field; show="$OPTARG" ;;
#%      -h
#%          show this help menu
			h) help ;;
#%      -i
#%          import from standard in
			i) import ;;
#%      -k
#%          print keys only
			k) cmd=echo ;;
#%      -l
#%          link to pdfs
         l) cmd=link ;;
#%      -o
#%          open pdf or url
			o) cmd=open ;;
#%      -p
#%          pretty print citations
         p) cmd=pretty ;;
#%      -r _file_
#%          select an alternative rsc file for bibtool
			r) rscfile="$OPTARG" ;;
#%      -s _searchterm_
#%          specify search criteria for entries
#%          multiple _-s_ flags can be passed for an 'or' pattern
#%          multiple search terms can be joined by an + for 'and' patterns
#%          search terms can be a single word, or a key=value pair
			s) keys="$keys $(search "$OPTARG")" ;;
#%      -v
#%          print selected entries - this is the default action
			v) cmd=view ;;
			-) ;;
			?) help "unrecognized option" ;;
		esac
	done
	shift $((OPTIND-1))
	keys="$keys $@"
	for key in $keys; do
		$cmd $key
	done
	[[ $post_clean -eq 1 ]] && clean
}

function field() {
	key=$1
	oldIFS=$IFS
	IFS=+
	for arg in $show; do
		[[ $arg == "key" ]] && echo $key
		[[ $arg != "key" ]] && entry $key | awk '
			/^ *'$arg' / {
				sub(/ *'$arg' *= *{/,"");
				sub(/},$/,"");
				printf "%s", $0;
				getline;
				while ($1 != "}" && $2 != "=") {
					sub(/^ */,"");
					sub(/},$/,"");
					printf " %s", $0;
					getline;
				}
				print "";
			}
		'
	done
	IFS=$oldIFS
}

#%
#%    *Actions*
function clean() {
	[[ ! -f $rscfile ]] && rsc
	bibtool -r $rscfile -i $bibfile -o $bibfile
}

#%      _delete_
#%          removes entries from the bibfile
function cut() {
	entry $1 > $tmpfile
	sed -i '/@.*{ *'$1',/,/^}$/d' $bibfile
}

#%      _edit_
#%          edit specified entries
function edit() {
	cut $1
	$bibeditor $tmpfile
	paste
}

function entry() {
	awk '/@.*{ *'$1',/ {P=1;} // {if (P) print $0;} P==1 && /^}/ {P=0; print "";}' $bibfile
}

function help() {
	[[ $1 ]] && echo -e "\033[31;1mError:\033[0m $1"
	sed \
		-e '/^#%/!d'	\
		-e 's/^#% \{0,1\}//'	\
		-e 's/\*\([^ ]*\)\*/'$(echo -e "\033[1m")'\1'$(echo -e "\033[0m")'/g'	\
		-e 's/_\([^ ]*\)_/'$(echo -e "\033[4m")'\1'$(echo -e "\033[0m")'/g'	\
		$0 #| /usr/bin/less
	[[ $1 ]] && exit 1 || exit 0
}

function import() {
	while read line; do echo $line >> $bibfile; done
}

function link() {
	if [[ -f ${pdfpath}${1}.pdf ]]; then
		cut $1
		sed -i '/^}/ i file = {'"${1}.pdf"'},' $tmpfile
		paste
	fi
}

function open() {
	show=file
	pdf=$(field $1)
	show=url
	url=$(field $1)
	[[ $pdf ]] && $pdfreader ${pdfpath}$pdf && return
	[[ $url ]] && $browser $url && return
}

function paste() {
	cat $tmpfile >> $bibfile
}

function pretty() {
	(
	show=author; author=$(field $1)
	show=year; year=$(field $1)
	show=title; title=$(field $1)
	show=journal; journal=$(field $1)
	show=volume; volume=$(field $1)
	show=number; number=$(field $1)
	show=pages; pages=$(field $1)
	printf "\033[1m%s\033[0m (%s) %s. \033[3m%s\033[0m " \
		"$author" "$year" "$title" "$journal"
	if [[ $journal ]]; then
		if [[ $number ]]; then
			printf "%s(%s)" "$volume" "$number"
		else
			printf "%s" "$volume"
		fi
		[[ $pages ]] && printf ": %s" "$pages"
	fi
	printf "\n"
	) | fold -s | awk 'NR==1 {print;} NR!=1 {print "    " $0;}'
}

function rsc() {
cat <<EOF > $rscfile
print.align					18
print.align.key			22
print.align.string      18
print.comma.at.end		On
print.indent				3
print.line.length			80
print.newline				1
print.terminal.comma		On
print.use.tab				Off
symbol.type             lower
sort                    On
fmt.inter.name          ""
fmt.et.al               ""
key.number.separator    ""
key.base                lower
key.generation          On
key.format              { %1p(author) %d(year) }
sort.order              { * =
                          title      # author   # organization  # year   #
                          journal    # volume   # number        # issue  # pages  #
                          booktitle  # edition  # editor        # issn   #
                          publisher  # address  #
                          file       # url      # doi
                        }
EOF
}

function search() {
	if [[ "$1" =~ .*\+.* ]]; then
		pre="${1%%+*}"
		post="${1#*+}"
		comm -12 <($0 -k -s $pre) <($0 -k -s $post)
		return
	fi

	awk_str='
		BEGIN {
			IGNORECASE = 1;
		}
		/^@[A-Za-z]*{/ {
			sub(/,/,"",$2);
			key=$2;
			keep=0;
		}
	'
	if [[ $1 =~ .*=.* ]]; then
		field="${1%%=*}"
		match="${1#*=}"
		awk_str="$awk_str"'
			/^ *'$field'/ {
				$1="";
				if (index($0, "'$match'"))
					keep=1;
				getline;
				while ($2 != "=") {
					if (index($0, "'$match'"))
						keep=1;
					getline;
				}
			}
			/^}/ {
				if (keep)
					print key;
			}
		'
	else
		awk_str="$awk_str"'
			/^[^}]/ {
				if ($2 == "=")
					$1="";
				if (index($0, "'$1'"))
					keep=1;
			}
			/^}/ {
				if (keep)
					print key;
			}

		'
	fi
	awk "$awk_str" $bibfile
}

#%      _view_
#%          print entries (default action)
function view() {
	[[ $viewpipe ]] && entry $1 | $viewpipe || entry $1
}

main $@


