
set tw=0

let WikiBase = 'https://wiki.archlinux.org/'
let RuleBase = WikiBase . 'index.php/Forum_Etiquette'
let SearchIt = WikiBase . 'index.php?title=Special%3ASearch&search'
let RuleSet = [
\	[ [ "ettiquette", "rules" ],
\		'[url=' . RuleBase . ']' .
\		'forum ettiquette' . '[/url]' ],
\
\	[ [ "introduction", "sense" ],
\		'[url=' . RuleBase . '#Common_Sense_Introduction]' .
\		'common sense introduction' . '[/url]' ],
\
\	[ [ "personal", "rant" ],
\		'[url=' . RuleBase . '#Personal_Topics.2FRants' .
\		'personal topics / rants' . '[/url]' ],
\
\	[ [ "ineffective", "bikeshed" ],
\		'[url=' . RuleBase . '#Ineffective_Discussion_.28BIKESHED.29' .
\		'ineffective discussion (bikeshed)' . '[/url]' ],
\
\	[ [ "responsible" ],
\		'[url=' . RuleBase . '#Be_Responsible' .
\		'be responsible' . '[/url]'],
\
\	[ [ "users", "respect", "other" ],
\		'[url=' . RuleBase . '#Respect_Other_Users' .
\		'respect other users' . '[/url]' ],
\
\	[ [ "distros", "other", "OS" ],
\		'[url=' . RuleBase .
\		'#Respect_Other_Distributions_and_Operating_Systems' .
\		'respect other distros and OSs' . '[/url]' ],
\
\	[ [ "paste", "pasting", "pictures", "code" ],
\		'[url=' . RuleBase . '#Pasting_Pictures_and_Code' .
\		'pasting pictures and code' . '[/url]' ],
\
\	[ [ "product", "buying", "recommendation" ],
\		'[url=' . RuleBase . '#Product_Recommendation_Requests' .
\		'product recommendation requests' . '[/url]' ],
\
\	[ [ "spam", "advertise", "solicitation" ],
\		'[url=' . RuleBase . '#Spam.2FAdvertising.2FSolicitation' .
\		'spam, advertising, & solicitation' . '[/url]' ],
\
\	[ [ "homework" ],
\		'[url=' . RuleBase . '#Homework' .
\		'homework' . '[/url]' ],
\
\	[ [ "necro", "old" ],
\		'[url=' . RuleBase . '#Old_Threads.2F.22Necro-Bumping.22' .
\		'"necrobumping" old threads' . '[/url]' ],
\
\	[ [ "accounts" ],
\		'[url=' . RuleBase . '#Forum_Member_Accounts' .
\		'forum accounts' . '[/url]' ],
\
\	[ [ "trolling" ],
\		'[url=' . RuleBase . '#No_Trolling' .
\		'trolling' . '[/url]' ],
\
\	[ [ "controversial" ],
\		'[url=' . RuleBase . '#Avoid_Controversy.2FControversial_Topics' .
\		'controversial topics' . '[/url]' ],
\
\	[ [ "power", "empty" ],
\		'[url=' . RuleBase . '#No_Power-Posting.2FEmpty_Posts' .
\		'power posting / empty posts' . '[/url]' ],
\
\	[ [ "two-way", "two", "way", "street" ],
\		'[url=' . RuleBase . '#Life_is_a_Two-way_Street' .
\		'life is a two-way street' . '[/url]' ],
\
\	[ [ "bumping" ],
\		'[url=' . RuleBase . '#Bumping' .
\		'bumping' . '[/url]' ],
\
\	[ [ "crosspost" ],
\		'[url=' . RuleBase . '#Cross-Posting' .
\		'cross post' . '[/url]' ],
\
\	[ [ "archlinux", "only" ],
\		'[url=' . RuleBase . '#Arch_Linux_Distribution_Support_ONLY' .
\		'arch linux support only' . '[/url]' ],
\
\	[ [ "misplaced", "subforum", "wrong" ],
\		'[url=' . RuleBase . '#Misplaced_Posts' .
\		'misplaced posts' . '[/url]' ],
\
\	[ [ "hijacking" ],
\		'[url=' . RuleBase . '#Thread_Hijacking' .
\		'hijack the thread' . '[/url]' ],
\
\	[ [ "flame" ],
\		'[url=' . RuleBase . '#Do_Not_Flame' .
\		'do not flame' . '[/url]' ],
\
\	[ [ "respect", "staff", "moderator", "admin" ],
\		'[url=' . RuleBase . '#Respect_The_Staff' .
\		'respect the staff' . '[/url]' ],
\
\	[ [ "warning", "locking", "banning" ],
\		'[url=' . RuleBase . '#Warnings.2C_User_Locking.2C_Banning' .
\		'warning, locking, banning' . '[/url]' ],
\
\	[ [ "how", "to", "how-to" ],
\		'[url=' . RuleBase . '#How_to_Post' .
\		'how to post' . '[/url]' ],
\
\	[ [ "dustbin", "deletion" ],
\		'[url=' . RuleBase . '#Dustbin_Policy_.28Marked_for_Deletion.29' .
\		'dustbin policy' . '[/url]' ],
\
\	[ [ "legality", "illegal" ],
\		'[url=' . RuleBase . '#Legality' .
\		'legality' . '[/url]' ],
\
\	[ [ "freedom" ],
\		'[url=' . RuleBase . '#Freedom' .
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
\		'help vampire' . '[/url]' ]
\	]


function MatchRule(...)
	let rule_num=0
	for rule in g:RuleSet
		let does_match=1
		for word in a:000
			if (match(rule[0], '\c' . word) == -1)
				let does_match=0
			endif
		endfor
		if (does_match == 1)
			break
		endif
		let rule_num=rule_num+1
	endfor
	if (rule_num < len(g:RuleSet))
		execute 'normal i' . g:RuleSet[rule_num][1]
	endif
endfunction

command -nargs=+ Rule call MatchRule(<f-args>)

