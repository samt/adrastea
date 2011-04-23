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
	init_f		init;
	send_resp_f send;
	resp_f		respond;
} modules_ht;

modules_ht * s_modules;
int s_modules_size = 0;

void run_event(irc_message m)
{
}

void load_modules()
{
	DIR * dip;
	struct dirent * dit;
	void * plugin;
	char filename[255];
	char * result;
	int i;

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

	s_modules = malloc(sizeof(modules_ht) * s_modules_size);

	while((dit = readdir(dip)) != NULL)
	{
		if(is_dot_so(dit->d_name))
		{
			strcpy(filename, "./lib/");
			strcat(filename, dit->d_name);
			printf(" - Loaded %s", dit->d_name);

			plugin = dlopen(filename, RTLD_NOW);
			result = dlerror();
			if(result)
			{
				error(result);
			}

			s_modules = (modules_ht*) realloc(s_modules, (sizeof(modules_ht) * ++s_modules_size));

			printf(" (%d)\n", (s_modules_size - 1));
			s_modules[s_modules_size - 1].init = dlsym(plugin, "init");
			result = dlerror();
			if(result)
			{
				error(result);
			}

			s_modules[s_modules_size - 1].send = dlsym(plugin, "send");
			result = dlerror();
			if(result)
			{
				error(result);
			}

			s_modules[s_modules_size - 1].respond = dlsym(plugin, "respond");
			result = dlerror();
			if(result)
			{
				error(result);
			}

			dlclose(plugin);
		}
	}

	if (closedir(dip) == -1)
	{
		printf("[WARNING] Directory did not properly close\n");
	}
}
