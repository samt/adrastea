/******************************************************************************
 * Adrastea IRC Bot
 *
 * Version 2
 * (c) 2011 Sam Thompson <sam@websyntax.net>
 * The MIT License
 *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "../src/adrastea.h"

/*
 * Initialization
 */
void init()
{
}

/*
 * Will this plugin respond to the event?
 * @return int - the number of lines to send
 */
int respond(irc_message * m)
{
	return streq(m->cmd, "PING") ? 1 : 0;
}

/*
 * Send off the messages 
 */
void send_responses(irc_message * m, char * msg[])
{
	sprintf(msg[0], "PONG :%s", m->message);
}
