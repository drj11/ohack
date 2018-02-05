/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* hack.mhitu.c version 1.0.1 - corrected bug for 'R' (Mike Newton)
			      - also some separate code for swallowed (a3) */
#include	"hack.h"
extern struct monst *makemon();

/*
 * mhitu: monster hits you
 *	  returns 1 if monster dies (e.g. 'y', 'F'), 0 otherwise
 */
mhitu(mtmp)
register struct monst *mtmp;
{
	register struct permonst *mdat = mtmp->data;
	register int tmp, ctmp;

	nomul(0);

	/* If swallowed, can only be affected by hissers and by u.ustuck */
	if(u.uswallow) {
		if(mtmp != u.ustuck && mdat->mlet != 'c')
			return(0);
		switch(mdat->mlet) {
		case 'c':
			if(!rn2(13)) {
				pline("Outside, you hear %s's hissing!",
					monnam(mtmp));
				pline("%s gets turned to stone!",
					Monnam(u.ustuck));
				pline("And the same fate befalls you.");
				done_in_by(mtmp);
			}
			break;
		case ',':
			youswld(mtmp,4+u.uac,5,"The trapper");
			break;
		case '\'':
			youswld(mtmp,rnd(6),7,"The lurker above");
			break;
		case 'P':
			youswld(mtmp,d(2,4),12,"The purple worm");
			break;
		default:
			pline("The mysterious monster digests you.");
			u.uhp = 0;
		}
		if(u.uhp < 1) done_in_by(mtmp);
		return(0);
	}
	if(!index("&DuxynNF",mdat->mlet))
		tmp = hitu(mtmp,d(mdat->damn,mdat->damd));
	else
		tmp = 0;

	ctmp = tmp && !mtmp->mcan &&
	  (!uarm || objects[uarm->otyp].a_can < rnd(3) || !rn2(50));
	switch(mdat->mlet) {
	case '&':
		if(!mtmp->cham && !mtmp->mcan && !rn2(13)) {
			(void) makemon(PM_DEMON,u.ux,u.uy);
		} else {
			(void) hitu(mtmp,d(2,6));
			(void) hitu(mtmp,d(2,6));
			(void) hitu(mtmp,rnd(3));
			(void) hitu(mtmp,rnd(3));
			(void) hitu(mtmp,rn1(4,2));
		}
		break;
	case ',':
		if(tmp) justswld(mtmp,"The trapper");
		break;
	case '\'':
		if(tmp) justswld(mtmp,"The lurker above");
		break;
	case 'A':
		if(ctmp && rn2(2)) {
			pline("You feel weaker!");
			losestr(1);
		}
		break;
	case 'C':
		(void) hitu(mtmp,rnd(6));
		break;
	case 'c':
		if(!rn2(5)) {
			pline("You hear %s's hissing!", monnam(mtmp));
			if(ctmp || !rn2(5)) {
				pline("You get turned to stone!");
				done_in_by(mtmp);
			}
		}
		break;
	case 'D':
		if(rn2(6) || mtmp->mcan) {
			(void) hitu(mtmp,d(3,10));
			(void) hitu(mtmp,rnd(8));
			(void) hitu(mtmp,rnd(8));
			break;
		}
		kludge("%s breathes fire!","The dragon");
		buzz(-1,mtmp->mx,mtmp->my,u.ux-mtmp->mx,u.uy-mtmp->my);
		break;
	case 'd':
		(void) hitu(mtmp,d(2,4));
		break;
	case 'e':
		(void) hitu(mtmp,d(3,6));
		break;
	case 'F':
		if(mtmp->mcan) break;
		kludge("%s explodes!","The freezing sphere");
		if(Cold_resistance) pline("You don't seem affected by it.");
		else {
			xchar dn;
			if(17-(u.ulevel/2) > rnd(20)) {
				pline("You get blasted!");
				dn = 6;
			} else {
				pline("You duck the blast...");
				dn = 3;
			}
 losehp_m(d(dn,6), mtmp);
		}
		mondead(mtmp);
		return(1);
	case 'g':
		if(ctmp && multi >= 0 && !rn2(6)) {
			kludge("You are frozen by %ss juices","the cube'");
			nomul(-rnd(10));
		}
		break;
	case 'h':
		if(ctmp && multi >= 0 && !rn2(5)) {
			nomul(-rnd(10));
			kludge("You are put to sleep by %ss bite!",
				"the homunculus'");
		}
		break;
	case 'j':
		tmp = hitu(mtmp,rnd(3));
		tmp &= hitu(mtmp,rnd(3));
		if(tmp){
			(void) hitu(mtmp,rnd(4));
			(void) hitu(mtmp,rnd(4));
		}
		break;
	case 'k':
		if((hitu(mtmp,rnd(4)) || !rn2(3)) && ctmp){
			poisoned("bee's sting",mdat->mname);
		}
		break;
	case 'L':
		if(tmp) stealgold(mtmp);
		break;
	case 'N':
		if(mtmp->mcan && !Blind) {
	pline("%s tries to seduce you, but you seem not interested.",
			Amonnam(mtmp, "plain"));
			if(rn2(3)) rloc(mtmp);
		} else if(steal(mtmp)) {
			rloc(mtmp);
			mtmp->mflee = 1;
		}
		break;
	case 'n':
		if(!uwep && !uarm && !uarmh && !uarms && !uarmg) {
		    pline("%s hits! (I hope you don't mind)",
			Monnam(mtmp));
			u.uhp += rnd(7);
			if(!rn2(7)) u.uhpmax++;
			if(u.uhp > u.uhpmax) u.uhp = u.uhpmax;
			flags.botl = 1;
			if(!rn2(50)) rloc(mtmp);
		} else {
			(void) hitu(mtmp,d(2,6));
			(void) hitu(mtmp,d(2,6));
		}
		break;
	case 'o':
		tmp = hitu(mtmp,rnd(6));
		if(hitu(mtmp,rnd(6)) && ctmp &&
		    !u.ustuck && rn2(2)) {
			u.ustuck = mtmp;
			kludge("%s has grabbed you!","The owlbear");
			u.uhp -= d(2,8);
		} else if(u.ustuck == mtmp) {
			u.uhp -= d(2,8);
			pline("You are being crushed.");
		}
		break;
	case 'P':
		if(ctmp && !rn2(4))
			justswld(mtmp,"The purple worm");
		else
			(void) hitu(mtmp,d(2,4));
		break;
	case 'Q':
		(void) hitu(mtmp,rnd(2));
		(void) hitu(mtmp,rnd(2));
		break;
	case 'R':
		if(tmp && uarmh && !uarmh->rustfree &&
		    (int) uarmh->spe >= -1) {
			pline("Your helmet rusts!");
			uarmh->spe--;
		} else
		if(ctmp && uarm && !uarm->rustfree &&
		 uarm->otyp < STUDDED_LEATHER_ARMOR &&
		 (int)uarm->spe >= -1) {
			pline("Your armor rusts!");
			uarm->spe--;
		}
		break;
	case 'S':
		if(ctmp && !rn2(8)) {
			poisoned("snake's bite",mdat->mname);
		}
		break;
	case 's':
		if(tmp && !rn2(8)) {
			poisoned("scorpion's sting",mdat->mname);
		}
		(void) hitu(mtmp,rnd(8));
		(void) hitu(mtmp,rnd(8));
		break;
	case 'T':
		(void) hitu(mtmp,rnd(6));
		(void) hitu(mtmp,rnd(6));
		break;
	case 't':
		if(!rn2(5)) rloc(mtmp);
		break;
	case 'u':
		mtmp->mflee = 1;
		break;
	case 'U':
		(void) hitu(mtmp,d(3,4));
		(void) hitu(mtmp,d(3,4));
		break;
	case 'v':
		if(ctmp && !u.ustuck) u.ustuck = mtmp;
		break;
	case 'V':
		if(tmp) u.uhp -= 4;
		if(ctmp && !rn2(3)) losexp();
		break;
	case 'W':
		if(ctmp && !rn2(5)) losexp();
		break;
#ifndef NOWORM
	case 'w':
		if(tmp) wormhit(mtmp);
#endif
		break;
	case 'X':
		(void) hitu(mtmp,rnd(5));
		(void) hitu(mtmp,rnd(5));
		(void) hitu(mtmp,rnd(5));
		break;
	case 'x':
		{ register long side = rn2(2) ? RIGHT_SIDE : LEFT_SIDE;
		  pline("%s pricks in your %s leg!",
			Monnam(mtmp), (side == RIGHT_SIDE) ? "right" : "left");
		  Wounded_legs |= side + rnd(5);
		  losehp_m(2, mtmp);
		  break;
		}
	case 'y':
		if(mtmp->mcan) break;
		mondead(mtmp);
		if(!Blind) {
			pline("You are blinded by a blast of light!");
			Blind = d(4,12);
			seeoff(0);
		}
		return(1);
	case 'Y':
		(void) hitu(mtmp,rnd(6));
		break;
	}
	if(u.uhp < 1) done_in_by(mtmp);
	return(0);
}
