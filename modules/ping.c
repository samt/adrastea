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
 * Called when the all modules are re/loaded
 */
void init()
{
	// Does nothing
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
 *
 * If the responce for respond() is greater than 0, this will be called, and it
 * expects the second param to be filled with goodies to send off.
 */
void send_responses(irc_message * m)
{
	printf("PONG :%s", m->message);
	irc_sendf("PONG :%s", m->message);
}

