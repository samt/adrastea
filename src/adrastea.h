/******************************************************************************
 * Adrastea IRC Bot
 *
 * Version 2
 * (c) 2011 Sam Thompson <sam@websyntax.net>
 * The MIT License
 *****************************************************************************/

#ifndef ADRASTEA_HEADER
#define ADRASTEA_HEADER

#define IRC_LINE_END "\r\n"
#define streq(s1,s2) (strcmp(s1,s2)==0)

int get_int_cfg(const char * name);
void get_str_cfg(const char * name, char * dest);
int irc_recv(char raw[]);
int irc_sendf(const char * msg, ...);
int irc_send(const char * msg);

int sock = 0;

typedef enum {
	EVENT_NONE,
	EVENT_MSG,
	EVENT_ACTION,
	EVENT_JOIN,
	EVENT_PART,
	EVENT_QUIT,
	EVENT_CHNICK
} event;

typedef enum {
	TYPE_SERVER,
	TYPE_CHANNEL,
	TYPE_USER
} event_type;

typedef enum {
	ACTION_MSG,
	ACTION_CTCP
} event_action;

typedef struct {
	char raw[512];
	char prefix[255];
	char cmd[10];
	char target[256];
	char message[512];
	char nick[31];
	char ctcp[16];
	char ctcpArg[512];
	event e;
} irc_message;

#endif
