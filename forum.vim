
set tw=0

let WikiBase = 'https://wiki.archlinux.org/'
let RuleBase = WikiBase . 'index.php/Forum_Etiquette'
let SearchIt = WikiBase . 'index.php?title=Special%3ASearch&search'
let BBS_List = [
\	[ [ "ettiquette", "rules" ],
\		'[url=' . RuleBase . ']' .
\		'forum ettiquette' . '[/url]' ],
\
\	[ [ "introduction", "sense" ],
\		'[url=' . RuleBase . '#Common-sense_introduction]' .
\		'common sense introduction' . '[/url]' ],
\
\	[ [ "personal", "rant" ],
\		'[url=' . RuleBase . '#Personal_topics.2Frants]' .
\		'personal topics / rants' . '[/url]' ],
\
\	[ [ "ineffective", "bikeshed" ],
\		'[url=' . RuleBase . '#Ineffective_discussion_.28.22bikeshed.22.29]' .
\		'ineffective discussion (bikeshed)' . '[/url]' ],
\
\	[ [ "responsible" ],
\		'[url=' . RuleBase . '#Be_responsible]' .
\		'be responsible' . '[/url]'],
\
\	[ [ "users", "respect", "other" ],
\		'[url=' . RuleBase . '#Respect_other_users]' .
\		'respect other users' . '[/url]' ],
\
\	[ [ "distros", "other", "OS" ],
\		'[url=' . RuleBase . '#Respect_other_distributions_and_operating_systems]' .
\		'respect other distros and OSs' . '[/url]' ],
\
\	[ [ "paste", "pasting", "pictures", "code" ],
\		'[url=' . RuleBase . '#Pasting_pictures_and_code]' .
\		'pasting pictures and code' . '[/url]' ],
\
\	[ [ "product", "buying", "recommendation" ],
\		'[url=' . RuleBase . '#Product_recommendation_requests]' .
\		'product recommendation requests' . '[/url]' ],
\
\	[ [ "spam", "advertise", "solicitation" ],
\		'[url=' . RuleBase . '#Spam.2Fadvertising.2Fsolicitation]' .
\		'spam, advertising, & solicitation' . '[/url]' ],
\
\	[ [ "homework" ],
\		'[url=' . RuleBase . '#Homework]' .
\		'homework' . '[/url]' ],
\
\	[ [ "necro", "old" ],
\		'[url=' . RuleBase . '#Old_threads.2F.22necro-bumping.22]' .
\		'"necrobumping" old threads' . '[/url]' ],
\
\	[ [ "accounts" ],
\		'[url=' . RuleBase . '#Forum_member_accounts]' .
\		'forum accounts' . '[/url]' ],
\
\	[ [ "trolling" ],
\		'[url=' . RuleBase . '#No_trolling]' .
\		'trolling' . '[/url]' ],
\
\	[ [ "controversial" ],
\		'[url=' . RuleBase . '#Avoid_controversy.2Fcontroversial_topics]' .
\		'controversial topics' . '[/url]' ],
\
\	[ [ "power", "empty" ],
\		'[url=' . RuleBase . '#No_power-posting.2Fempty_posts]' .
\		'power posting / empty posts' . '[/url]' ],
\
\	[ [ "two-way", "two", "way", "street" ],
\		'[url=' . RuleBase . '#Life_is_a_two-way_street]' .
\		'life is a two-way street' . '[/url]' ],
\
\	[ [ "bumping" ],
\		'[url=' . RuleBase . '#Bumping]' .
\		'bumping' . '[/url]' ],
\
\	[ [ "crosspost" ],
\		'[url=' . RuleBase . '#Cross-posting]' .
\		'cross post' . '[/url]' ],
\
\	[ [ "archlinux", "only" ],
\		'[url=' . RuleBase . '#Arch_Linux_distribution_support_ONLY]' .
\		'arch linux support only' . '[/url]' ],
\
\	[ [ "misplaced", "subforum", "wrong" ],
\		'[url=' . RuleBase . '#Misplaced_posts]' .
\		'misplaced posts' . '[/url]' ],
\
\	[ [ "hijacking" ],
\		'[url=' . RuleBase . '#Thread_hijacking]' .
\		'hijack the thread' . '[/url]' ],
\
\	[ [ "flame" ],
\		'[url=' . RuleBase . '#Do_not_flame]' .
\		'do not flame' . '[/url]' ],
\
\	[ [ "respect", "staff", "moderator", "admin" ],
\		'[url=' . RuleBase . '#Respect_the_staff]' .
\		'respect the staff' . '[/url]' ],
\
\	[ [ "warning", "locking", "banning" ],
\		'[url=' . RuleBase . '#Warnings.2C_user_locking.2C_banning]' .
\		'warning, locking, banning' . '[/url]' ],
\
\	[ [ "how", "to", "how-to" ],
\		'[url=' . RuleBase . '#How_to_post]' .
\		'how to post' . '[/url]' ],
\
\	[ [ "dustbin", "deletion" ],
\		'[url=' . RuleBase . '#Dustbin_policy_.28marked_for_deletion.29]' .
\		'dustbin policy' . '[/url]' ],
\
\	[ [ "legality", "illegal" ],
\		'[url=' . RuleBase . '#Legality]' .
\		'legality' . '[/url]' ],
\
\	[ [ "freedom" ],
\		'[url=' . RuleBase . '#Freedom]' .
\		'freedom' . '[/url]' ],
\
\	[ [ "search", "STFW" ],
\		'[url=' . SearchIt . ']' .
\		'search the wiki' . '[/url]' ],
\
\	[ [ "beginner's", "guide" ],
\		'[url=' . WikiBase .'index.php/Beginner%27s_Guide]' .
\		"beginner's guide" . '[/url]' ],
\
\	[ [ "questions", "ESR" ],
\		'[url=http://catb.org/~esr/faqs/smart-questions.html]' .
\		'effective questions' . '[/url]' ],
\
\	[ [ "vampire", "help", "slash7" ],
\		'[url=http://slash7.com/2006/12/22/vampires/]' .
\		'help vampire' . '[/url]' ],
\
\	[ [ "tags" ],
\		'[url=https://bbs.archlinux.org/help.php#bbcode]' .
\		'code tags' . '[/url]' ],
\
\	[ [ "bbcode" ],
\		'[url=https://bbs.archlinux.org/help.php#bbcode]' .
\		'bbcode' . '[/url]' ],
\	]


function MatchBBS(...)
	let entry_num=0
	for BBS_Entry in g:BBS_List
		let does_match=1
		for word in a:000
			if (match(BBS_Entry[0], '\c' . word) == -1)
				let does_match=0
			endif
		endfor
		if (does_match == 1)
			break
		endif
		let entry_num=entry_num+1
	endfor
	if (entry_num < len(g:BBS_List))
		execute 'normal a ' . g:BBS_List[entry_num][1]
	endif
endfunction

command -nargs=+ BBS call MatchBBS(<f-args>)

