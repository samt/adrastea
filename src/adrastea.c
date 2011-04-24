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
#define CONFIG_FILE "bot.conf"

#include "adrastea.h"
#include "functions.c"
#include "config.c"
#include "server.c"
#include "modules.c"

/**
 * Main function
 */
int main()
{
	int port;
	char raw[512];
	char buffer[512];
	char host[255], nick[255], name[255], user[255], pass[255], channels[512];
	irc_message msg;

	(void) signal(SIGINT,interupt);

	/* Print our headers out */
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
	get_str_cfg("pass", pass);
	get_str_cfg("channels", channels);
	port = get_int_cfg("port");

	printf("Loading modules...\n");
	load_modules();

	/* Connection loop */
	for(;;)
	{
		/* Hit that */
		irc_connect(host, port);

		/* Send our connection details */
		irc_sendf("NICK %s", nick);
		irc_sendf("USER %s * 8 :%s", user, name);

		/* Message loop */
		while(irc_recv(raw) > 0)
		{
			printf("%s\n", raw);
			parse_message(&msg, raw);
			run_event(&msg);

			if (0)
			{
				quit('e');
			}
		}

		printf("Network error, reconnecting...\n");
	}

	return 0;
}
