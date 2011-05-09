/******************************************************************************
 * Adrastea IRC Bot
 *
 * Version 2
 * (c) 2011 Sam Thompson <sam@websyntax.net>
 * The MIT License
 *****************************************************************************/

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/**
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

	printf("Connecting to %s:%d...\n", (char*) inet_ntoa(*((struct in_addr *)server->h_addr)), port);
	if (connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	{
		error("Could not connect to server");
	}
}

/**
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
	raw[--i] = raw[--i] = '\0';

	return strlen(raw);
}

/**
 * Send a line
 */
int irc_send(const char * msg)
{
	char buffer[strlen(msg) + 2];
	
	strcpy(buffer, msg);
	strcat(buffer, IRC_LINE_END);

	write(sock, buffer, strlen(buffer));
	return strlen(buffer);
}

/**
 * Send a line (but with a format)
 */
int irc_sendf(const char * fmt, ...)
{
	char buffer[512] = {""};
	va_list vl;

	va_start(vl,fmt);
	vsprintf(buffer, fmt, vl);	
	va_end (vl);

	return irc_send(buffer);
}
