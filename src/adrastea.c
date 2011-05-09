/******************************************************************************
 * Adrastea IRC Bot
 *
 * Version 2
 * (c) 2011 Sam Thompson <sam@websyntax.net>
 * The MIT License
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdarg.h>
#include <dlfcn.h>

#define BUILD_NUM 3
#define MODULES_DIR "./lib/"

#include "adrastea.h"
#include "config.c"
#include "server.c"
#include "parse.c"
#include "modules.c"

/**
 * Main function
 */
int main()
{
	int port, queue_len, i;
	char raw[512];
	char buffer[512];
	char host[255], nick[255], name[255], user[255], channels[512];
	char ** queue;
	irc_message msg;

	printf("================================================================================\n");
	printf(" Adrastea IRC Bot\n\n");
	printf(" Version 2\n");
	printf(" Build %d\n", BUILD_NUM);
	printf(" (c) 2011 Sam Thompson <sam@websyntax.net>\n");
	printf(" The MIT License <http://www.opensource.org/licenses/mit-license.php>\n");
	printf("================================================================================\n");

	/* Get our config ready to go */
	printf("Loading Configuration Settings...\n");
	load_config();

	get_str_cfg("host", host);
	get_str_cfg("nick", nick);
	get_str_cfg("name", name);
	get_str_cfg("user", user);
	get_str_cfg("channels", channels);
	port = get_int_cfg("port");

	printf("Loading modules...\n");
	load_modules();

	(void) signal(SIGINT,interupt);

	// the loop within a loop
	for(;;)
	{
		// Connect and send off our info
		irc_connect(host, port);

		irc_sendf("NICK %s", nick);
		irc_sendf("USER %s * 8 :%s", user, name);

		while(irc_recv(raw) > 0 && sock > 0)
		{
			printf("%s\n", raw);
			parse_message(&msg, raw);

			// Dispatch event and get the queue going
			queue = (char**) malloc(1);
			queue_len = dispatch_event(&msg, queue);
			for(i = 0; i < queue_len; i++)
			{
				irc_send(queue[i]);
				printf("%s\n", queue[i]);
			}
			free(queue);

			if(flag == FLAG_QUIT)
			{
				quit();
			}
		}
	}

	return 0;
}

/*
 * Generic error
 */
void error(const char * msg)
{
	if(sock != 0)
	{
		close(sock);
	}

	printf("[ERROR] %s\n", msg);
	exit(1);
}

/*
 * Generic formated error
 */
void errorf(const char * fmt, ...)
{
	char buffer[512] = {""};
	va_list vl;

	va_start(vl,fmt);
	vsprintf(buffer, fmt, vl);	
	va_end (vl);

	error(buffer);
}

/*
 * Quit helper
 */
void _quit()
{
	char quit_msg[128];
	char buffer[512];

	printf("Shutting down bot...\n");

	unload_modules();

	get_str_cfg("quitmsg", quit_msg);
	irc_sendf("QUIT :%s", quit_msg);

	while(irc_recv(buffer) > 0)
	{
		printf("%s\n", buffer);
	}

	close(sock);
}

/*
 * Callback for the interupt signal
 */
void interupt(int sig)
{
	char buffer[512];

	printf("\n");
	printf("Interruption detected.\n");

	if(sock != 0)
	{
		_quit();
	}

	exit(sig);
}

/*
 * Quit
 */
void quit()
{
	_quit();
	exit(0);
}
