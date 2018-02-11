/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* savelev.h version 1.0.1 - also save engravings from MKLEV */

#include <stddef.h>
#include <stdio.h>

extern struct monst *restmonchn();
extern struct obj *restobjchn();
void savegenchn(int, FILE *, struct gen *);
void saveobjchn(int, FILE *, struct obj *);
struct obj *billobjs;
extern char *itoa();

extern char nul[];
#ifndef NOWORM

struct wseg *wsegs[32], *wheads[32];
long wgrowtime[32];
#endif

savelev(fd){
#ifndef NOWORM
	register struct wseg *wtmp, *wtmp2;
	register int tmp;
#endif

	if(fd < 0)
           panic("Save on bad file!");

        FILE *log = fdopen(9, "wb");
        fprintf(log, "levl: %zd, %zd\n",
            sizeof levl[0][0], sizeof levl);
	bwrite(fd, levl,sizeof(levl));
	bwrite(fd, &moves,sizeof(long));
	bwrite(fd, &xupstair,sizeof(xupstair));
	bwrite(fd, &yupstair,sizeof(yupstair));
	bwrite(fd, &xdnstair,sizeof(xdnstair));
	bwrite(fd, &ydnstair,sizeof(ydnstair));
	savemonchn(fd, log, fmon);
	savegenchn(fd, log, fgold);
	savegenchn(fd, log, ftrap);
	saveobjchn(fd, log, fobj);
	saveobjchn(fd, log, billobjs);
        billobjs = 0;
#ifndef QUEST
	bwrite(fd,(char *) rooms,sizeof(rooms));
	bwrite(fd,(char *) doors,sizeof(doors));
#endif
	save_engravings(fd);

	   fgold = ftrap = 0;
	   fmon = 0;
	   fobj = 0;

/*--------------------------------------------------------------------*/
#ifndef NOWORM
	bwrite(fd,(char *) wsegs,sizeof(wsegs));
	for(tmp=1; tmp<32; tmp++){
		for(wtmp = wsegs[tmp]; wtmp; wtmp = wtmp2){
			wtmp2 = wtmp->nseg;
			bwrite(fd,(char *) wtmp,sizeof(struct wseg));
		}
/*		if (!ismklev) */
		   wsegs[tmp] = 0;
	}
	bwrite(fd,(char *) wgrowtime,sizeof(wgrowtime));
#endif
/*--------------------------------------------------------------------*/
}

bwrite(int fd, void *loc, unsigned num)
{
/* lint wants the 3rd arg of write to be an int; lint -p an unsigned */
	if(write(fd, loc, num) != num)
		panic("cannot write %d bytes to file #%d",num,fd);
}

void
saveobjchn(int fd, FILE *log, struct obj *otmp)
{
	register struct obj *otmp2;
	unsigned xl;

        fprintf(log, "obj %zu\n", sizeof *otmp);
	while(otmp) {
		otmp2 = otmp->nobj;
		xl = otmp->onamelth;
		bwrite(fd, &xl, sizeof xl);
		bwrite(fd, otmp, xl + sizeof *otmp);
		free(otmp);
		otmp = otmp2;
	}
        xl = -1;
	bwrite(fd, &xl, sizeof xl);
}

savemonchn(int fd, FILE *log, struct monst *mtmp)
{
	struct monst *mtmp2;
	unsigned xl;

	fprintf(log, "monst %zu, %zu\n", sizeof xl, sizeof *mtmp);
	while(mtmp) {
                ptrdiff_t monnum;
		mtmp2 = mtmp->nmon;
		xl = mtmp->mxlth + mtmp->mnamelth;
		bwrite(fd, &xl, sizeof xl);

		/* JAT - just save the offset into the monster table, */
		/* it will be relocated when read in */
		fprintf(log, "Writing %s\n", mtmp->data->mname);
		monnum = mtmp->data - &mons[0];
		mtmp->data = (struct permonst *)monnum;
		bwrite(fd, mtmp, xl + sizeof *mtmp);
		if(mtmp->minvent) saveobjchn(fd, log, mtmp->minvent);
		free(mtmp);
		mtmp = mtmp2;
	}
        xl = -1;
	bwrite(fd,  &xl, sizeof xl);
}

void
savegenchn(int fd, FILE *log, struct gen *gtmp)
{
        unsigned long c = 0;
	register struct gen *gtmp2;
	while(gtmp) {
                c += 1;
		gtmp2 = gtmp->ngen;
		bwrite(fd, gtmp, sizeof *gtmp);
                free( gtmp);
		gtmp = gtmp2;
	}
        fprintf(log, "gen %zu, count %lu\n", sizeof *gtmp, c);
	bwrite(fd, nul, sizeof(struct gen));
}


