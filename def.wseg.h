/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1984. */

#ifndef NOWORM
/* worm structure */
struct wseg {
	struct wseg *nseg;
	xchar wx,wy;
	unsigned wdispl:1;
};

#define newseg()	(struct wseg *) alloc(sizeof(struct wseg))
#endif
