/**********************************************************\
* LAGOPUS:
* Author: Jesse McClure, 2012-2014
* License: released to public domain (attribution appreciated)
* Example status input for alopex v4
*
* COMPILE:
*   $ sed -i 's/jmcclure/'$USER'/' lagopus.c
*   $ gcc -o lagopus lagopus.c
*
\**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

/* input files */
static const char *CPU_FILE		= "/proc/stat";
static const char *MEM_FILE		= "/proc/meminfo";
static const char *AUD_FILE		= "/proc/asound/card0/codec#0";
static const char *WIFI_FILE		= "/proc/net/wireless";
static const char *BATT_NOW		= "/sys/class/power_supply/BAT0/energy_now";
static const char *BATT_FULL	= "/sys/class/power_supply/BAT0/energy_full";
static const char *BATT_STAT	= "/sys/class/power_supply/BAT0/status";
static const char *MAIL_BASE	= "/home/jmcclure/mail/";
static const char *MAIL_1		= "mccluresk9/INBOX/";
static const char *MAIL_2		= "umass/INBOX/";

/* icon names: may need to be adjusted to match your icon set */
enum {
	none,
	tag0, tag1, tag2, tag3, tag4, tag5, tag6, tag7, tag8, tag9,
	cpu0, cpu1, cpu2, cpu3, cpu4, cpu5, cpu6, cpu7, cpu8, cpu9,
	mem0, mem1, mem2, mem3, mem4, mem5, mem6, mem7, mem8, mem9,
	aud0, aud1, aud2, aud3, aud4, aud5, aud6, aud7, aud8, aud9,
	bat0, bat1, bat2, bat3, bat4, bat5, bat6, bat7, bat8, b_AC,
	wfi0, wfi1, wfi2, wfi3, wfi4, wfi5, wfi6, wfi7, wfi8, wfi9,
	clk0, clk1, clk2, eml0, eml1, eml2,
};

/* variables */
static long		j1,j2,j3,j4,ln1,ln2,ln3,ln4;
static int		n, loops = 0, mail = 0;
static char		c, clk[8], *aud_file, *mail_file;
static FILE		*in;
static time_t	current;

static int mailcheck(const char *accnt) {
	DIR *dir;
	struct dirent *de;
	int mail = 0;
	chdir(MAIL_BASE);
	chdir(accnt);
	if ( !(dir=opendir("new")) ) return 0;
	while ( (de=readdir(dir)) )
		if (de->d_name[0] != '.') { mail = 1; break; }
	closedir(dir);
	return mail;
}

int main(int argc, const char **argv) {
	in = fopen(CPU_FILE,"r");
	fscanf(in,"cpu %ld %ld %ld %ld",&j1,&j2,&j3,&j4);
	fclose(in);
	/* main loop */
	for (;;) {
		if ( (in=fopen(CPU_FILE,"r")) ) {       /* CPU MONITOR */
			fscanf(in,"cpu %ld %ld %ld %ld",&ln1,&ln2,&ln3,&ln4);
			fclose(in);
			if (ln4>j4)
				n=(int)100*(ln1-j1+ln2-j2+ln3-j3)/(ln1-j1+ln2-j2+ln3-j3+ln4-j4);
			else n=0;
			j1=ln1; j2=ln2; j3=ln3; j4=ln4;
			printf("&& {i%d}",(n/10)+cpu0);
		}
		if ( (in=fopen(MEM_FILE,"r")) ) {		/* MEM USAGE MONITOR */
			fscanf(in,"MemTotal: %ld kB\nMemFree: %ld kB\n"
					"MemAvailable: %ld kB", &ln1,&ln2,&ln3);
			fclose(in);
			n = 100 - 100*ln3/ln1;
			printf("{i%d}",(n/10)+mem0);
		}
		if ( (in=fopen(AUD_FILE,"r")) ) {       /* AUDIO VOLUME MONITOR */
			while ( fscanf(in," Amp-Out caps: ofs=0x%x",&ln1) !=1 )
				fscanf(in,"%*[^\n]\n");
			while ( fscanf(in," Amp-Out vals: [0x%x",&ln2) != 1 )
				fscanf(in,"%*[^\n]\n");
			fclose(in);
			printf("{i%d}",9*ln2/ln1 + aud0);
		}
		if ( (in=fopen(BATT_NOW,"r")) ) {       /* BATTERY MONITOR */
			fscanf(in,"%ld\n",&ln1); fclose(in);
			if ( (in=fopen(BATT_FULL,"r")) ) {
				fscanf(in,"%ld\n",&ln2); fclose(in);
			}
			if ( (in=fopen(BATT_STAT,"r")) ) {
				fscanf(in,"%c",&c); fclose(in);
			}
			n = (ln1 ? ln1 * 100 / ln2 : 0);
			if (c == 'C') printf("{i%d}",b_AC);
			else if (n < 5) printf("{i%d}",bat0);
			else if (n < 10) printf("{i%d}",bat1);
			else if (n < 16) printf("{i%d}",bat2);
			else if (n < 23) printf("{i%d}",bat3);
			else if (n < 31) printf("{i%d}",bat4);
			else if (n < 40) printf("{i%d}",bat5);
			else if (n < 50) printf("{i%d}",bat6);
			else if (n < 61) printf("{i%d}",bat7);
			else if (n < 73) printf("{i%d}",bat8);
			/* 73-100 = no inicator */
		}
		if ( (in=fopen(WIFI_FILE,"r")) ) {       /* WIFI MONITOR */
			n = 0;
			fscanf(in,"%*[^\n]\n%*[^\n]\n wls1: %*d %d.",&n);
			fclose(in);
			if (n > 0) printf("{i%d}",(n/10)+wfi0);
		}
		if ((loops % 20) == 0)						/* MAIL */
			mail = 2*mailcheck(MAIL_2) + mailcheck(MAIL_1);
			if (mail == 1) printf("{{i%d} ",eml0);
			else if (mail == 2) printf("{i%d} ",eml1);
			else if (mail == 3) printf("{i%d} ",eml2);
		if ((loops % 80) == 0) {					/* TIME */
			time(&current);
			strftime(clk,6,"%H:%M",localtime(&current));
		}
		if (loops++ > 600) {
			loops = 0;
		}
		printf(" %s &\n",clk);
		fflush(stdout);
		usleep(500000);
	}
	return 0;
}
