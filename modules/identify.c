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
	if(starts_with(m->message, "This nickname is registered") && streq("NickServ", m->nick))
	{
		char pw[255];
		get_str_cfg("pass", pw);
		
		responces[0] = malloc(28 + strlen(pw));
		sprintf(responces[0], "PRIVMSG NickServ :IDENTIFY %s", pw);
		return 1;
	}
	else
	{
		return 0;
	}
}
