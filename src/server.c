/******************************************************************************
 * Adrastea IRC Bot
 *
 * Version 2
 * (c) 2011 Sam Thompson <sam@websyntax.net>
 * The MIT License
 *****************************************************************************/

/*
 * Connect
 * @param const char ptr - Host
 * @param int - Port
 */
void irc_connect(const char * host, int port)
{
	struct hostent *server;
	struct sockaddr_in serv_addr;

	/* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		error("Could not create socket");
	}

	/* Look up host */
	printf("Looking up %s...\n", host);
	server = gethostbyname(host);
	if(server == NULL)
	{
		error("Lookup failed");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);

	/* Connect to that shit */
	printf("Connecting to %s:%d...\n", (char*) inet_ntoa(*((struct in_addr *)server->h_addr)), port);
	if (connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	{
		error("Could not connect to server");
	}
}

/*
 * Recv a line
 */
int irc_recv(char raw[])
{
	char b;
	int i = 0;
	memset(raw, '\0', 512);

	do
	{
		if(read(sock, &b, 1) < 1)
		{
			return 0;
		}

		raw[i] = b;
		i++;
	}
	while (b != '\n');

	// Be rid of the last two bytes (\r\n)
	raw[i - 1] = raw[i - 2] = '\0';

	return strlen(raw);
}

/*
 * Send a line
 */
int irc_send(const char * msg)
{
	char tosend[strlen(msg) + 2];
	
	strcpy(tosend, msg);
	strcat(tosend, IRC_LINE_END);

	write(sock, tosend, strlen(tosend));
	return strlen(tosend);
}

int irc_sendf(const char * msg, ...)
{
	char tosend[512] = {""};
	int i, len, b = 0;
	va_list vl;

	va_start(vl,msg);
	vsprintf(tosend, msg, vl);	
	va_end (vl);

	return irc_send(tosend);
}
