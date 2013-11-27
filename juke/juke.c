
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

typedef struct _Song { char *file; int flags; } Song;

static int repeat = 0, running = 1, pid;

void received(int sig) {
	kill(pid,SIGKILL);
	if (sig == SIGQUIT) running = 0;
	else if (sig == SIGUSR1) return;
	else if (sig == SIGUSR2) repeat = 1;
}

int main(int argc, const char **argv) {
	FILE *list;
	list = popen("find ~/music -type f | sort -R","r");
	char line[256];
	Song *songs = NULL;
	int n = 0, i, status;
	while (fgets(line,256,list)) {
		line[strlen(line)-1] = '\0';
		songs = realloc(songs,(n+1)*sizeof(Song));
		songs[n].file = strdup(line);
		songs[n].flags = 1;
		for (i = 1; i < argc; i++)
			if (!strcasestr(line,argv[i])) songs[n].flags = 0;
		n++;
	}
	pclose(list);
	signal(SIGQUIT,&received);
	signal(SIGUSR1,&received);
	signal(SIGUSR2,&received);
	char *args[5];
	args[0] = "/usr/bin/mpv"; args[3] = NULL;
	i=-1;
	while (running) {
		if (repeat) { i--; repeat = 0; }
		for (i++; i < n && !songs[i].flags; i++);
		if (i == n) { i = -1; continue; }
		args[1] = songs[i].file;
		if ( (pid=fork()) == 0 ) execv(args[0],(char * const *)args);
		while ( waitpid(pid, &status, 0) == -1 );
	}
	while (n) free(songs[n--].file);
	free(songs);
	return;
}
