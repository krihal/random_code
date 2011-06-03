/** 
 ** Small lib for commonly used C functions.
 **
 **  -- Kristofer Hallin kristofer.hallin@gmail.com
 **
 */

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <signal.h>
#include <execinfo.h>

#ifndef BASELIB_H
#define BASELIB_H
#endif /* BASELIB_H */

FILE *dbg_fd;

#ifndef STACK_MAX_SIZE
#define STACK_MAX_SIZE 10
#endif

#define BITMASK_CLEAR(b)	(b &= ~0xFFFFFFFF)
#define BITMASK_SET(b,v)	(b |= v)
#define BITMASK_TEST(b,v)	if(b & v)
#define BITMASK_UNSET(b,v)	(b &= v)	

#ifndef ASSERT_INTERACTIVE
#define ASSERT(c) \
	if(c) ; else { ASSERT_FAILURE(c); kill(0, 6); }

#define ASSERT_FAILURE(c) \
	fprintf(stderr, "%s: assertion %s failed on line %d in %s\n", \
			__func__, #c, __LINE__, __FILE__);
#endif /* ASSERT_INTERACTIVE */

#ifdef ASSERT_INTERACTIVE
#define ASSERT(c) \
	if(c) ; else { ASSERT_INTERACTIVE(c); }

#define ASSERT_INTERACTIVE \
	int ch = 0; printf("Assertion detected, (I)gnore or (A)bort?\n> "); \
	ch = getchar(); if (ch == 'I') ; else if (ch == 'A') kill(0,6);
#endif /* ASSERT_INTERACTIVE */

#define DEBUG_FILE_INIT	\
	ASSERT(dbg_fd = fopen("debug.log", "w+"));

#define DEBUG_FILE(x) \
	fprintf(dbg_fd, "%d, %s: %s\n", time(NULL), __func__, x);

#define DEBUG_FILE_CLOSE \
	fclose(dbg_fd);

#define DEBUG(x) \
	fprintf(stderr, "%d, %s: %s\n", time(NULL), __func__, x);

#define ERROR_RET -1

#define ERROR(x) \
	fprintf(stderr, "%d, %s: %s\n", time(NULL), __func__, x); \
	exit(ERROR_RET);

#define MIN(a,b) \
	(a > b ? a : b)

#define MAX(a,b) \
	(b > a ? b : a)

#define FLIP(a,b) \
	a ^= b; b ^= a; a ^= b;

void stacktrace_init(void);
static void stacktrace_print(const int sig);

