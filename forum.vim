
set tw=0

let RuleBase = 'https://wiki.archlinux.org/index.php/Forum_Etiquette'
let RuleString = [
\	'[url=' . RuleBase . ']' .
\		'forum ettiquette' . '[/url]',
\	'[url=' . RuleBase . '#Common_Sense_Introduction]' .
\		'common sense introduction' . '[/url]',
\	'[url=' . RuleBase . '#Personal_Topics.2FRants' .
\		'personal topics / rants' . '[/url]',
\	'[url=' . RuleBase . '#Ineffective_Discussion_.28BIKESHED.29' .
\		'ineffective discussion (bikeshed)' . '[/url]',
\	'[url=' . RuleBase . '#Be_Responsible' .
\		'be responsible' . '[/url]',
\	'[url=' . RuleBase . '#Respect_Other_Users' .
\		'respect other users' . '[/url]',
\	'[url=' . RuleBase .
\		'#Respect_Other_Distributions_and_Operating_Systems' .
\		'respect other distros and OSs' . '[/url]',
\	'[url=' . RuleBase . '#Pasting_Pictures_and_Code' .
\		'pasting pictures and code' . '[/url]',
\	'[url=' . RuleBase . '#Product_Recommendation_Requests' .
\		'product recommendation requests' . '[/url]',
\	'[url=' . RuleBase . '#Spam.2FAdvertising.2FSolicitation' .
\		'spam, advertising, & solicitation' . '[/url]',
\	'[url=' . RuleBase . '#Homework' .
\		'homework' . '[/url]',
\	'[url=' . RuleBase . '#Old_Threads.2F.22Necro-Bumping.22' .
\		'"necrobumping" old threads' . '[/url]',
\	'[url=' . RuleBase . '#Forum_Member_Accounts' .
\		'forum accounts' . '[/url]',
\	'[url=' . RuleBase . '#No_Trolling' .
\		'trolling' . '[/url]',
\	'[url=' . RuleBase . '#Avoid_Controversy.2FControversial_Topics' .
\		'controversial topics' . '[/url]',
\	'[url=' . RuleBase . '#No_Power-Posting.2FEmpty_Posts' .
\		'power posting / empty posts' . '[/url]',
\	'[url=' . RuleBase . '#Life_is_a_Two-way_Street' .
\		'life is a two-way street' . '[/url]',
\	'[url=' . RuleBase . '#Bumping' .
\		'bumping' . '[/url]',
\	'[url=' . RuleBase . '#Cross-Posting' .
\		'cross post' . '[/url]',
\	'[url=' . RuleBase . '#Arch_Linux_Distribution_Support_ONLY' .
\		'arch linux support only' . '[/url]',
\	'[url=' . RuleBase . '#Misplaced_Posts' .
\		'misplaced posts' . '[/url]',
\	'[url=' . RuleBase . '#Thread_Hijacking' .
\		'hijack the thread' . '[/url]',
\	'[url=' . RuleBase . '#Do_Not_Flame' .
\		'do not flame' . '[/url]',
\	'[url=' . RuleBase . '#Respect_The_Staff' .
\		'respect the staff' . '[/url]',
\	'[url=' . RuleBase . '#Warnings.2C_User_Locking.2C_Banning' .
\		'warning, locking, banning' . '[/url]',
\	'[url=' . RuleBase . '#How_to_Post' .
\		'how to post' . '[/url]',
\	'[url=' . RuleBase . '#Dustbin_Policy_.28Marked_for_Deletion.29' .
\		'dustbin policy' . '[/url]',
\	'[url=' . RuleBase . '#Legality' .
\		'legality' . '[/url]',
\	'[url=' . RuleBase . '#Freedom' .
\		'freedom' . '[/url]',
\	]
let WikiString = 'https://wiki.archlinux.org/'
let GuideString = [
\	'[url=' . WikiString .'index.php?title=Special%3ASearch&search]' .
\		'search the wiki' . '[/url]'
\	'[url=' . WikiString .'index.php/Beginner%27s_Guide]' .
\		"beginner's guide" . '[/url]'
\	'[url=http://catb.org/~esr/faqs/smart-questions.html]'
\		'effective questions' . '[/url]'
\	'[url=http://slash7.com/2006/12/22/vampires/]'
\		'help vampire' . '[/url]'
\	]

command -nargs=1 Rule execute 'normal i' . g:RuleString[<args>]
command -nargs=1 Guide execute 'normal i' . g:GuideString[<args>]

