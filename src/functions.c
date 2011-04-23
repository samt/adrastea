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

	if(strlen(msg->prefix) > 0 && msg->e != EVENT_NONE)
	{
		memset(msg->nick, '\0', 31);
		for (i = 0; msg->prefix[i] != '!'; i++);
		strncpy(msg->nick, msg->prefix, i);
	}
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
 * Quit
 * @param char flag - e: exit; d: don't exit
 */
void quit(char flag)
{
	char quit_msg[128];
	char buffer[512];
	
	printf("Shutting down bot...\n");
	get_str_cfg("quitmsg", quit_msg);
	irc_sendf("QUIT :%s", quit_msg);

	while(irc_recv(buffer) > 0)
	{
		printf("%s\n", buffer);
	}

	close(sock);

	if (flag == 'e')
	{
		exit(0);
	}
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
		quit('d');
	}

	exit(sig);
}