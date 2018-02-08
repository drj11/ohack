#include <stdlib.h>

long *
alloc(lth)
register unsigned lth;
{
	register void *ptr;

	if(!(ptr = malloc(lth)))
		panic("Cannot get %d bytes", lth);
	return(ptr);
}

long *
enlarge(ptr,lth)
register char *ptr;
register unsigned lth;
{
	register void *nptr;

	if(!(nptr = realloc(ptr,lth)))
		panic("Cannot reallocate %d bytes", lth);
	return(nptr);
}

