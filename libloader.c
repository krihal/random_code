/*
 * Simple dynamic library loader (C) Kristofer Hallin, 2008.
 * Libs must be compiled as shared libraries and will
 * be called by the __lib_init and __lib_exit functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <dlfcn.h>
#include <string.h>

typedef struct{
	void *handler;
	void (*init)(void);
	void (*exit)(void);
	unsigned int flags:2;
#define PLUGIN_LOADED  0x01
#define LIB_STARTED 0x04
	char *name;
} lib;

int lib_init(lib *plg)
{
	plg->flags &= ~0xFFFFFFFF;

	return 0;
}

int lib_load(lib *plg, char *name)
{
	if(plg->flags & LIB_LOADED || plg->flags & LIB_STARTED) {
		fprintf(stderr, "lib already loaded\n");
		return -1;
	}

	plg->name = name;
	plg->handler = dlopen(name, RTLD_NOW);
	if(plg->handler == NULL) {
		fprintf(stderr, "failed to load lib (%s)\n", dlerror());
		return -1;
	}

	/* Load the symbols __lib_init and __lib_exit into the
	   lib structure. We then have functions to call if needed */
       
	plg->init = dlsym(plg->handler, "__lib_init");
	plg->exit = dlsym(plg->handler, "__lib_exit");
	plg->flags |= LIB_LOADED;

	return 0;
}

int lib_start(lib *plg)
{
	if(plg->flags & LIB_STARTED || !plg->flags & LIB_LOADED ) {
		fprintf(stderr, "lib not loaded\n");
		return -1;
	}

	plg->flags |= LIB_STARTED;
	plg->init();
	
	/* We're now in the hands of the lib, so don't expect this
	   function to always return a value. */

	return 0;
}

int lib_unload(lib *plg)
{
	if(!plg->flags & LIB_LOADED) {
		printf("lib not loaded, no need to unload\n");
		return -1;
	}

	plg->exit();

	/* Best effort */
	dlclose(plg->handler);

	plg->init = NULL;
	plg->exit = NULL;
	plg->flags &= LIB_STARTED;
	plg->flags &= LIB_LOADED;

	return 0;
}
