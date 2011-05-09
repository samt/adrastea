/******************************************************************************
 * Adrastea IRC Bot
 *
 * Version 2
 * (c) 2011 Sam Thompson <sam@websyntax.net>
 * The MIT License
 *****************************************************************************/
#include <dirent.h>

typedef struct {
	void * plugin;
	init_f		init;
	resp_f		respond;
} modules_ht;

modules_ht * s_modules;
int s_modules_size = 0;

/*
 * Load Modules
 * @return int - 1 on success, 2 on failure
 */
int load_modules()
{
	DIR * dip;
	struct dirent * dit;
	char filename[255];
	char * result;
	int c, i;

	if((dip = opendir(MODULES_DIR)) == NULL)
	{
		error("Unable to load the open the 'lib' directory to load modules");
	}

	// Make sure we're starting over if we're reloading
	if(s_modules_size)
	{
		free(s_modules);
		s_modules_size = 0;
	}

	// read through the directory 
	while((dit = readdir(dip)) != NULL)
	{
		if(dot_so(dit->d_name))
		{
			// Get our real path ready
			strcpy(filename, "./lib/");
			strcat(filename, dit->d_name);
	
			// Increase the size of our hash table
			c = s_modules_size;
			s_modules_size++;
			s_modules = (modules_ht*) realloc(s_modules, (sizeof(modules_ht) * s_modules_size));

			// Open and error check
			s_modules[c].plugin = dlopen(filename, RTLD_NOW);
			result = dlerror();
			if(result)
			{
				error(result);
			}

			// Grab init function and error check
			s_modules[c].init = dlsym(s_modules[c].plugin, "init");
			result = dlerror();
			if(result)
			{
				error(result);
			}

			// Grab our responces and error check
			s_modules[c].respond = dlsym(s_modules[c].plugin, "respond");
			result = dlerror();
			if(result)
			{
				error(result);
			}

			// Upon a successful load, tell it to the world
			printf(" - Loaded %s\n", dit->d_name);
		}
	}

	if (closedir(dip) == -1)
	{
		printf("[WARNING] Directory did not properly close\n");
	}

	/* Run all inits */
	for(i = 0; i < s_modules_size; i++)
	{
		s_modules[i].init();
	}
}

/*
 * Dispatch event
 * @param irc_message* - Pointer to the current IRC message
 * @param char** - Pointer to an array of chars
 * @return int - number of items in the array to send
 */
int dispatch_event(irc_message * msg, char ** queue)
{
	int i, j, n, total = 0;
	char ** q;

	for(i = 0; i < s_modules_size; i++)
	{
		q = malloc(sizeof(char*));
		n = s_modules[i].respond(msg, q);

		// unload the temp queue into the main queue
		if(n > 0)
		{
			queue = (char**) realloc(queue, (n + total));

			for(j = 0; j < n; j++)
			{
				queue[total] = malloc(strlen(q[j]));
				strcpy(queue[total], q[j]);
				free(q[j]);
				total++;
			}
		}

		free(q);
	}

	return total;
}

/*
 * Unload modules
 */
void unload_modules()
{
	int i;
	for(i = 0; i < s_modules_size; i++)
	{
		dlclose(s_modules[i].plugin);
	}
}
