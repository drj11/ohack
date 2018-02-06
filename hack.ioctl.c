/* This cannot be part of hack.tty.c (as it was earlier) since on some
   systems (e.g. MUNIX) the include files <termio.h> and <sgtty.h>
   define the same constants, and the C preprocessor complains. */
#include <stdio.h>
#include "config.h"

#include	<termio.h>

struct termio termio;

getioctls() {
	(void) ioctl(fileno(stdin), (int) TCGETA, &termio);
}

setioctls() {
	(void) ioctl(fileno(stdin), (int) TCSETA, &termio);
}


