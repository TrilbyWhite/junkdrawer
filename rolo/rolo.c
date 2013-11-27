
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE	255
#define DEX_FILE	".rolo.dex"

typedef struct Entry Entry;
struct Entry {
	Entry *prev, *next;
	char *first, *middle, *last;
	char *address;
	char *city;
	char state[3];
	unsigned int zip;
	char *email;
	unsigned long int phone;
	struct {
		unsigned short int day, month;
		unsigned int year;
	} birth;
	unsigned long int flags;
	char *notes;
	char **group;
};

typedef struct {
	Entry *e;
	char **group;
	int ng;
} Dex;

static int add_email(Dex *);
static int email(Dex *, int, const char **);
static int free_dex(Dex *);
static int get_emails(Dex *,const char *);
static int new_entry(Dex *);
static Dex *read_dex(const char *);
static int sort(Dex *);
static int tag_list(Dex *,int, const char **);
static int write_dex(Dex *,const char *);

static inline int check_tag(Dex *d, Entry *e, const char *str) {
	int i;
	for (i = 0; i < 16; i++)
		if ( ((e->flags>>i) & 1) && !strncasecmp(d->group[i],str,strlen(str)) )
			return 1;
	return 0;
}

static inline void draw_entry(Dex *d, Entry *e,FILE *out,int evenodd) {
	fprintf(out,
"\033[38;5;%dm\n"
"%s %s %s%*s%s\n"
"%s%*s%0.10lu\n"
"%s %s %0.5u%*s%hu/%hu/%u\n"
"\033[0m",
(evenodd ? 255 : 247),
e->first, e->middle, e->last,
	26 - (strlen(e->first)+strlen(e->middle)+strlen(e->last)), " ",
	e->email,
e->address,
	28 - (strlen(e->address)), " ",
	e->phone,
e->city, e->state, e->zip,
	19 - strlen(e->city), " ",
e->birth.month, e->birth.day, e->birth.year);
}

static inline int match(Entry *e, const char *str) {
	return (strcasestr(e->last,str) || strcasestr(e->first,str) || strcasestr(e->email,str));
}

int add_email(Dex *d) {
	Entry *l, *e;
	char line[MAX_LINE+1], last[MAX_LINE], first[MAX_LINE], email[MAX_LINE];
	first[0] = '\0';
	while (fgets(line,MAX_LINE,stdin)) {
		if (sscanf(line,"From: %s %s <%[^>]>",first, last, email) == 3 ||
				sscanf(line,"From: %[^<]<%[^>]>",last, email) == 2)
			break;
	}
	char *t;
	if (first[0] == '"') { t = strdup(first+1);strcpy(first,t);free(t);}
	if (last[0] == '"') { t = strdup(first+1);strcpy(first,t);free(t);}
	if (first[strlen(first)-1] == '"') first[strlen(first) - 1] == '\0';
	if (first[strlen(last)-1] == '"') first[strlen(last) - 1] == '\0';
	for (e = d->e; e; e = e->next) {
		l = e;
		if (strncasecmp(email,e->email,strlen(email)) == 0) return 1;
	}
	l->next = e = (Entry *) calloc(1,sizeof(Entry));
	e->last = strdup(last);
	if (strlen(first) > 1) e->first = strdup(first);
	else e->first = strdup("~");
	e->email = strdup(email);
	e->middle = strdup("~");
	e->address = strdup("~");
	e->city = strdup("~");
	e->state[0] = '~'; e->state[1] = '~';
	e->notes = strdup("~");
	return 0;
}

int email(Dex *d, int argc, const char **argv) {
	if (argv[1][0] == 'a') add_email(d);
	else if (argv[1][0] == 'g' && argc > 2) get_emails(d,argv[2]);
}

int free_dex(Dex *d) {
	Entry *t, *e = d->e;
	int i;
	for (i = 0; i < d->ng; i++) free(d->group[i]);
	free(d->group);
	while (e) {
		free(e->last); free(e->first); free(e->middle);
		free(e->address); free(e->city); free(e->email);
		free(e->notes);
		t = e->next; free(e); e = t;
	}
	return 0;
}

int get_emails(Dex *d,const char *str) {
	Entry *e;
	int m, i;
	printf("Search results for \"%s\"\n",str);
	for (e = d->e; e; e = e->next) {
		m = match(e,str);
		if (!m) for (i = 0; i < d->ng; i++)
			if (((e->flags>>i) & 1) && !strncasecmp(d->group[i],str,strlen(str)))
				m = 1;
		if (m) printf("%s\t%s %s\n",e->email,e->first,e->last);
	}
}

static int new_entry(Dex *d) {
	
	printf("todo\n");
}

Dex *read_dex(const char *fname) {
	FILE *in = fopen(fname,"r");
	char s1[MAX_LINE], s2[MAX_LINE], s3[MAX_LINE];
	Dex *d = (Dex *) calloc(1,sizeof(Dex));
	d->e = (Entry *) malloc(sizeof(Entry));
	Entry *e = d->e, *t = NULL;
	char c1,c2;
	while (fgets(s1,MAX_LINE,in) && (s1[0] != '<'));
	for (;;) {
		fscanf(in,"%s\n",s1);
		if (s1[0] == ']') break;
		d->group = (char **) realloc(d->group,(d->ng+1) * sizeof(char *));
		d->group[d->ng++] = strdup(s1);
	}
	while (fgets(s1,MAX_LINE,in)) {
		if (s1[0] != '=') continue;
		fscanf(in,"%[^|]|%[^|]|%[^\n]\n",s1,s2,s3);
		e->last = strdup(s1); e->first = strdup(s2); e->middle = strdup(s3);
		fscanf(in,"%[^|]|%[^|]|%c%c|%u\n",s1,s2,&e->state[0],&e->state[1],&e->zip);
		e->address = strdup(s1); e->city = strdup(s2); e->state[2] = '\0';
		fscanf(in,"%[^\n]\n%lu\n",s1,&e->phone);
		e->email = strdup(s1);
		fscanf(in,"%hu/%hu/%u\n",&e->birth.month,&e->birth.day,&e->birth.year);
		if (fscanf(in,"%X\nNOTES: %[^\n]\n",&e->flags,s1) == 2)
			e->notes = strdup(s1);
		else
			e->notes = NULL;
		e->prev = t;
		e->next = (Entry *) malloc(sizeof(Entry));
		t = e; e = e->next;
	}
	t->next = NULL;
	free(e);
	fclose(in);
	return d;
}

int sort(Dex *d) {
	int i, loop = 1;
	Entry *e, *n, t;
	while (loop) {
		loop = 0;
		for (e = d->e; e && e->next; e = e->next) {
			i = strcmp(e->last,e->next->last);
			if (i > 0 || ( i==0 && strcmp(e->first,e->next->first) > 0) ) {
				t.last = e->last; t.first = e->first; t.middle = e->middle;
				t.address = e->address; t.city = e->city;
				strcpy(t.state,e->state);
				t.zip = e->zip; t.email = e->email; t.phone = e->phone;
				t.birth.day = e->birth.day; t.birth.month = e->birth.month;
				t.birth.year = e->birth.year; t.flags = e->flags;
				n = e->next;
				e->last = n->last; e->first = n->first; e->middle = n->middle;
				e->address = n->address; e->city = n->city;
				strcpy(e->state,n->state);
				e->zip = n->zip; e->email = n->email; e->phone = n->phone;
				e->birth.day = n->birth.day; e->birth.month = n->birth.month;
				e->birth.year = n->birth.year; e->flags = n->flags;
				n->last = t.last; n->first = t.first; n->middle = t.middle;
				n->address = t.address; n->city = t.city;
				strcpy(n->state,t.state);
				n->zip = t.zip; n->email = t.email; n->phone = t.phone;
				n->birth.day = t.birth.day; n->birth.month = t.birth.month;
				n->birth.year = t.birth.year; n->flags = t.flags;
				loop = 1;
			}	
		}
	}
	return 0;
}

int tag_list(Dex *d,int argc, const char **argv) {
	int i, n;
	for (n = 0; n < d->ng; n++)
		if (!strncasecmp(d->group[n],argv[1],strlen(argv[1])))
			break;
	if (n==d->ng) return 1;
	Entry *e;
	for (i = 1; i < argc; i++) {
		for (e = d->e; e; e = e->next)
			if (match(e,argv[i])) e->flags |= (1<<n);
	}
}

int write_dex(Dex *d,const char *fname) {
	FILE *out = fopen(fname,"w");
	int i;
	fprintf(out,"<[[ GROUPS\n");
	for (i = 0; i < d->ng; i++)
		fprintf(out,"%s\n",d->group[i]);
	fprintf(out,"]]>\n\n");
	Entry *e;
	for (e = d->e; e; e = e->next) fprintf(out,
		"=\n%s|%s|%s\n%s|%s|%s|%u\n%s\n"
		"%0.10lu\n%hu/%hu/%u\n%0.16X\nNOTES: %s\n\n",
		e->last,e->first,e->middle,e->address,e->city,e->state,e->zip,
		e->email,e->phone,e->birth.month,e->birth.day,e->birth.year,
		e->flags,e->notes);
	return 0;
}


int main(int argc, const char **argv) {
	chdir(getenv("HOME"));
	Dex *d = read_dex(DEX_FILE);
	Entry *e;
	int i = 0;
	if (argc == 1)
		for (e = d->e; e; e = e->next)
			draw_entry(d,e,stdout,(i++)%2);
	else {
		char c = argv[1][0];
		if (c == 'e') /* email */
			email(d,argc-1,&argv[1]);
		else if (argc == 2 && (c == 't')) /* list tags */
			for (i = 0; i < d->ng; i++)
				printf("%3d: %s\n",i,d->group[i]);
		else if (argc == 2 && (c == 'n' || c == 'e')) /* new entry */
			new_entry(d);
		else if (argc == 3 && (c == 'g')) { /* new tag */
			d->group = (char **) realloc(d->group,(d->ng+1) * sizeof(char *));
			d->group[d->ng++] = strdup(argv[2]);
		}
		else if (argc == 3 && (c == 'f' || c == 's')) /* find/search */
			for (e = d->e; e; e = e->next) {
				if (match(e,argv[2]))
					draw_entry(d,e,stdout,(i++)%2);
			}
		else if (argc == 3 && c == 't') /* show tag holders */
			for (e = d->e; e; e = e->next) {
				if (check_tag(d,e,argv[2]))
					draw_entry(d,e,stdout,(i++)%2);
			}
		else if (argc > 3 && c == 't') /* set tag */
			tag_list(d,argc - 1,&argv[1]);
		sort(d);
		write_dex(d,DEX_FILE);
	}	
	free_dex(d);
	return 0;
}



