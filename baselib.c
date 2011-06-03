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

#include "baselib.h"

void stacktrace_init(void)
{
	struct sigaction sa;

	sigemptyset(&(sa.sa_mask));
	sa.sa_handler = &stacktrace_print;
	sa.sa_flags = SA_RESETHAND;

	sigaction(SIGSEGV, &sa, NULL);
}

static void stacktrace_print(const int sig)
{
	void *stack_array[10];

	ssize_t stack_size;
	ssize_t stack_count;

	char **strings;

	stack_size = backtrace(stack_array, STACK_MAX_SIZE);
	
	fprintf(stderr, "*** Obtained %d stack frames\n", stack_size);

	for (stack_count = 0; stack_count < stack_size; stack_count++) {
		fprintf(stderr, "*** %s\n", strings[stack_count]);
	}

	free(strings);
}

