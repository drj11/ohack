/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* def.trap.h version 1.0.1 - added ONCE flag */

/* various kinds of traps */
#define BEAR_TRAP	0
#define	ARROW_TRAP	1
#define	DART_TRAP	2
#define TRAPDOOR	3
#define	TELEP_TRAP	4
#define PIT 5
#define SLP_GAS_TRAP	6
#define	PIERC	7
#define	MIMIC	8	/* used only in mklev.c */
/* before adding more trap types, check mfndpos ! */
/* #define SEEN 040 - trap which has been seen */
/* #define ONCE 0100 - once only trap */
