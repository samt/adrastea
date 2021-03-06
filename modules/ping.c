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

/*
 * Initialization
 * Called when the all modules are re/loaded
 */
void init()
{
}

/*
 * Will this plugin respond to the event?
 * @return int - the number of lines to send
 */
int respond(irc_message * m, char ** responces)
{
	if(streq(m->cmd, "PING"))
	{
		responces[0] = malloc(7 + strlen(m->message));
		sprintf(responces[0], "PONG :%s", m->message);
		return 1;
	}
	else
	{
		return 0;
	}
}
