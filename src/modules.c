/******************************************************************************
 * Adrastea IRC Bot
 *
 * Version 2
 * (c) 2011 Sam Thompson <sam@websyntax.net>
 * The MIT License
 *****************************************************************************/

typedef void (*init_f) ();
typedef void (*send_responses_f) ();
typedef int (*respond_f) ();

void run_event(irc_message m)
{
}

void load_modules()
{
	init_f init;
	send_responses_f send_responces;
	respond_f respond;
	void *plugin;
	char *result;

	plugin = dlopen("./lib/ping.so", RTLD_NOW);
	result = dlerror();
	if(result) error(result);

	init = dlsym (plugin, "init");
	result = dlerror();
	if(result) error(result);

	init();
}
