/******************************************************************************
 * Adrastea IRC Bot
 *
 * Version 2
 * (c) 2011 Sam Thompson <sam@websyntax.net>
 * The MIT License
 *****************************************************************************/
#include <dirent.h>

#define is_dot_so(fn) (strcmp(fn+strlen(fn)-3,".so")==0)

typedef void (*init_f) ();
typedef void (*send_resp_f) ();
typedef int (*resp_f) ();

typedef struct {
	void * plugin;
	init_f		init;
	send_resp_f send;
	resp_f		respond;
	int 		number;
} modules_ht;

modules_ht * s_modules;
int s_modules_size = 0;

void run_event(irc_message * m)
{
	int i, j, k = 0, total = 0;
	char ** queue;

	// Find the ones that will respond to the event 
	for(i = 0; i < s_modules_size; i++)
	{
		s_modules[i].number = s_modules[i].respond(m);
		total += s_modules[i].number;
		//	s_modules[i].send(m);
	}

	queue = malloc(total);

	// Get and Queue the responces
	for(i = 0; i < s_modules_size; i++)
	{
		if(s_modules[i].number > 0)
		{
			for(j = 0; j < s_modules[i].number; j++)
			{
				queue[k] = malloc(512);
				s_modules[i].send(m, queue[k]);
				k++;
			}
		}
	}

	// Send it off
	for(i = 0; i < total; i++)
	{
		irc_send(queue[i]);
	}

	free(queue);
}

void load_modules()
{
	DIR * dip;
	struct dirent * dit;
	char filename[255];
	char * result;
	int i, c;

	if((dip = opendir("./lib/")) == NULL)
	{
		error("Unable to load the open the 'lib' directory to load modules");
	}

	// Make sure we're starting over if we're reloading
	if(s_modules_size)
	{
		free(s_modules);
		s_modules_size = 0;
	}

	//s_modules = malloc(sizeof(modules_ht) * s_modules_size);

	while((dit = readdir(dip)) != NULL)
	{
		if(is_dot_so(dit->d_name))
		{
			strcpy(filename, "./lib/");
			strcat(filename, dit->d_name);

			s_modules = (modules_ht*) realloc(s_modules, (sizeof(modules_ht) * ++s_modules_size));
			c = s_modules_size - 1;

			s_modules[c].plugin = dlopen(filename, RTLD_NOW);
			result = dlerror();
			if(result)
			{
				error(result);
			}

			s_modules[c].init = dlsym(s_modules[c].plugin, "init");
			result = dlerror();
			if(result)
			{
				error(result);
			}

			s_modules[c].send = dlsym(s_modules[c].plugin, "send_responses");
			result = dlerror();
			if(result)
			{
				error(result);
			}

			s_modules[c].respond = dlsym(s_modules[c].plugin, "respond");
			result = dlerror();
			if(result)
			{
				error(result);
			}

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

void unload_modules()
{
	int i;
	for(i = 0; i < s_modules_size; i++)
	{
		dlclose(s_modules[i].plugin);
	}
}

