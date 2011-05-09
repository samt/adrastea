/******************************************************************************
 * Adrastea IRC Bot
 *
 * Version 2
 * (c) 2011 Sam Thompson <sam@websyntax.net>
 * The MIT License
 *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../src/adrastea.h"
#include "../src/config.c"

/*
 * Initialization
 * Called when the all modules are re/loaded
 */
void init()
{
	load_config();
}

/*
 * Will this plugin respond to the event?
 * @return int - the number of lines to send
 */
int respond(irc_message * m, char ** responces)
{
	if(starts_with(m->message, "You are now identified") && streq("NickServ", m->nick))
	{
		char joins[512];
		get_str_cfg("channels", joins);
		
		responces[0] = malloc(6 + strlen(joins));
		sprintf(responces[0], "JOIN %s", joins);
		return 1;
	}
	else
	{
		return 0;
	}
}
