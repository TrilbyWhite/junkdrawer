
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

int cprint(int fg, int bg, int attr, const char *fmt, ...) {
	if (fg == -1 && bg == -1) printf("\001\033[0;%dm\002", attr);
	else if (bg == -1) printf("\001\033[%d;38;5;%dm\002", attr, fg);
	else printf("\001\033[%d;38;5;%d;48;5;%dm\002", attr, fg, bg);
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	return 0;
}

static int cpu_col[] = {
	240, 242, 244, 246, 35,   46,  82, 118, 154, 190,
	226, 208, 202, 196, 160, 124, 124,  88,  88,  52,
};
static inline void check_cpu() {
	double avg;
	getloadavg(&avg, 1);
	avg *= 10;
	int i;
	for (i = 0; i < avg; ++i)
		cprint(cpu_col[i], -1, 1, "");
}

static const char *mailpath[] = {
	/* modify these as needed: */
	"/home/USERNAME/.mail/ACCOUNT1/Inbox/new",
	"/home/USERNAME/.mail/ACCOUNT2/Inbox/new",
	"/home/USERNAME/.mail/ACCOUNT3/Inbox/new",
}
static inline void check_mail(int fg, int bg) {
	struct dirent *dp; DIR *fd; int i, nmail;
	const char *mp;
	for (mp = mailpath[i=0]; i < sizeof(mailpath)/sizeof(mailpath[0]); mp = mailpath[++i]) {
		if ( !(fd = opendir(mp)) ) return;
		nmail = 0;
		while ( (dp=readdir(fd)) ) if (dp->d_name[0] != '.') ++nmail;
		closedir(fd);
		if (nmail) cprint(fg, bg, 0, "(%d new mail)", nmail);
	}
}

int main(int argc, const char **argv) {
	time_t now;
	struct tm *tinfo;
	time(&now);
	tinfo = localtime(&now);
	int i, col = 1;
	char line[256], sym = '$';
	if (argv[1][0] == '0') col = 4;
	if (getuid() == 0) sym = '#';
	printf("\n");
	char *ranger = getenv("RANGER_LEVEL");
	if (ranger) {
		cprint(15, col, 1, " ");
		for (i=atoi(ranger); i; --i)
			cprint(15, col, 1, "");
	}
	cprint(15, col, 1, " %03d ", atoi(argv[2])); cprint(col, 14, 0, "");
	strftime (line, 80, "%H:%M", tinfo);
	cprint(238, 14, 1, " %s ", line);
	cprint(14, 4, 0, "");
	scanf("%[^\n]\n", line); cprint(15, 4, 1, " %s ", line);
	while (scanf("%[^\n]\n", line) == 1) cprint(232, 4, 1, " %s ", line);
	cprint(4, 233, 0, "");
	cprint(237, 233, 0, "   %s   ", getenv("USER"));
	strftime (line, 80, "%a %b %d", tinfo);
	cprint(240, 233, 0, ""); cprint(233, 240, 0, " %s ", line);
	check_mail(233,240);
	cprint(240, -1, 0, "");

	check_cpu();

	cprint(-1, -1, 1, "\n%c", sym);
	cprint(-1, -1, 0, " ");
	return 0;
}
