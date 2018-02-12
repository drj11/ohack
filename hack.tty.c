/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1984. */

#include	"hack.h"
#include	<stdio.h>
#include <stdlib.h>
#include	<termios.h>

struct termios initerm, curterm;


gettty(){
	(void) tcgetattr(0, &initerm);
	(void) tcgetattr(0, &curterm);
	getioctls();
	xtabs();
}

/* reset terminal to original state */
settty(s) char *s; {
	clear_screen();
	if(s) printf("%s", s);
	(void) fflush(stdout);
	if(tcsetattr(0, TCSADRAIN, &initerm) == -1) puts("Cannot change tty");
	flags.echo = (initerm.c_lflag & ECHO) ? ON : OFF;
	flags.cbreak = (initerm.c_lflag & ICANON) ? OFF : ON;
	setioctls();
}

setctty(){
	if(tcsetattr(0, TCSADRAIN, &curterm) == -1) puts("Cannot change tty");
}

setftty(){
register int ef = (flags.echo == ON) ? ECHO : 0;
register int cf = (flags.cbreak == ON) ? 0 : ICANON;
register int change = 0;
	if((curterm.c_lflag & ECHO) != ef){
		curterm.c_lflag &= ~ECHO;
		curterm.c_lflag |= ef;
		change++;
	}
	if((curterm.c_lflag & ICANON) != cf){
		curterm.c_lflag &= ~ICANON;
		curterm.c_lflag |= cf;
		change++;
	}
	if(change){
		setctty();
	}
}

echo(n)
register n;
{
	if(n == ON)
		curterm.c_lflag |= ECHO;
	else
		curterm.c_lflag &= ~ECHO;
	setctty();
}

/* always want to expand tabs, or to send a clear line char before
   printing something on topline */
xtabs()
{

	/* (void) gtty(0, &curttyb); */
	// curttyb.sg_flags |= XTABS;
	setctty();
}

#ifdef LONG_CMD
cbreak(n)
register n;
{

	/* (void) gtty(0,&curttyb); */
	if(n == ON)
		curterm.l_cflag &= ~ICANON;
	else
		curterm.l_cflag |= ICANON;
	setctty();
}
#endif

getlin(bufp)
register char *bufp;
{
	register char *obufp = bufp;
	register int c;

	flags.topl = 2;		/* nonempty, no --More-- required */
	for(;;) {
		(void) fflush(stdout);
		if((c = getchar()) == EOF) {
			*bufp = 0;
			return;
		}
		if(c == '\b') {
			if(bufp != obufp) {
				bufp--;
				putstr("\b \b"); /* putsym converts \b */
			} else	bell();
		} else if(c == '\n') {
			*bufp = 0;
			return;
		} else {
			*bufp = c;
			bufp[1] = 0;
			putstr(bufp);
			if(bufp-obufp < BUFSZ-1 && bufp-obufp < COLNO)
				bufp++;
		}
	}
}

getret() {
	xgetret(TRUE);
}

cgetret() {
	xgetret(FALSE);
}

xgetret(spaceflag)
boolean spaceflag;	/* TRUE if space (return) required */
{
	printf("\nHit %s to continue: ",
		flags.cbreak ? "space" : "return");
	xwaitforspace(spaceflag);
}

char morc;	/* tell the outside world what char he used */

xwaitforspace(spaceflag)
boolean spaceflag;
{
register int c;

	(void) fflush(stdout);
	morc = 0;

	while((c = getchar()) != '\n') {
	    if(c == EOF) {
		settty("End of input?\n");
		exit(0);
	    }
	    if(flags.cbreak) {
		if(c == ' ') break;
		if(!spaceflag && letter(c)) {
			morc = c;
			break;
		}
  }
	}
}

char *
parse()
{
	static char inpline[COLNO];
	register foo;

	flags.move = 1;
	if(!Invis) curs(u.ux,u.uy+2); else home();
	(void) fflush(stdout);
	while((foo = getchar()) >= '0' && foo <= '9')
		multi += 10*multi+foo-'0';
	if(multi) {
		multi--;
		save_cm = inpline;
	}
	inpline[0] = foo;
	inpline[1] = 0;
	if(foo == EOF) {
		settty("End of input?\n");
		exit(0);
	}
	if(foo == 'f' || foo == 'F'){
		inpline[1] = getchar();
		inpline[2] = 0;
	}
	if(foo == 'm' || foo == 'M'){
		inpline[1] = getchar();
		inpline[2] = 0;
	}
	clrlin();
	return(inpline);
}

char
readchar() {
	register int sym;
	(void) fflush(stdout);
	if((sym = getchar()) == EOF) {
		settty("End of input?\n");
		exit(0);
	}
	if(flags.topl == 1) flags.topl = 2;
	return((char) sym);
}
