/******************************************************************************
 * Adrastea IRC Bot
 *
 * Version 2
 * (c) 2011 Sam Thompson <sam@websyntax.net>
 * The MIT License
 *****************************************************************************/

#include <string.h>
#include <stdio.h>

/*
 * Parse a raw IRC message
 */
void parse_message(irc_message * msg, const char * raw)
{
	char buffer[512];
	int i, len, offset = 0;

	memset(buffer, '\0', 512);
	memset(msg->prefix, '\0', 255);
	memset(msg->cmd, '\0', 10);
	memset(msg->target, '\0', 256);
	memset(msg->message, '\0', 512);
	memset(msg->nick, '\0', 31);
	memset(msg->ctcp, '\0', 16);
	memset(msg->ctcpArg, '\0', 512);
	msg->e = EVENT_NONE;

	strcpy(msg->raw, raw);
	strcpy(buffer, raw);

	// Find the next space
	for (i = 0; buffer[i] != ' '; i++);

	// Pull off the prefix if we have one
	if (buffer[0] == ':')
	{
		strncpy(msg->prefix, buffer + 1, i - 1);
		strcpy(buffer, buffer + i + 1);
		for (i = 0; buffer[i] != ' '; i++);
	}

	// Cmd, we will have one
	strncpy(msg->cmd, buffer, i);
	strcat(msg->cmd, "\0");
	strcpy(buffer, buffer + i + 1);

	if(buffer[0] != ':')
	{
		len = strlen(buffer);
		for (i = 0; !(buffer[i] == ':' && buffer[i-1] == ' ') && i < len; i++);

		if(buffer[i] == ' ') offset = 1;
	
		strncpy(msg->target, buffer, i - offset);
		strcpy(msg->message, buffer + i + 1);
	}
	else
	{
		strcpy(msg->message, buffer + 1);
	}

	if(strlen(msg->message) > 0 && msg->message[0] == '\1')
	{
		for (i = 0; buffer[i] =! ' ' || buffer[i] != '\1'; i++);

		strncpy(msg->ctcp, buffer + 1, i);

		if(buffer[i] == ' ')
		{
			for (i = 0; buffer[i] =! ' ' || buffer[i] != '\1'; i++);
			strncpy(msg->ctcpArg, buffer + 2 + strlen(msg->ctcp), i);
		}
	}

	if (streq(msg->ctcp,"ACTION"))
	{
		msg->e = EVENT_ACTION;
	}
	else if (streq(msg->cmd, "PRIVMSG"))
	{
		msg->e = EVENT_MSG;
	}
	else if (streq(msg->cmd, "JOIN"))
	{
		msg->e = EVENT_JOIN;
	}
	else if (streq(msg->cmd, "PART"))
	{
		msg->e = EVENT_PART;
	}
	else if (streq(msg->cmd, "QUIT"))
	{
		msg->e = EVENT_QUIT;
	}
	else if (streq(msg->cmd, "NICK"))
	{
		msg->e = EVENT_CHNICK;
	}

	if(strlen(msg->prefix) > 0 && strchr(msg->prefix, '!') != NULL)
	{
		memset(msg->nick, '\0', 31);
		for (i = 0; msg->prefix[i] != '!'; i++);
		strncpy(msg->nick, msg->prefix, i);
	}
}
