/******************************************************************************
 * Adrastea IRC Bot
 *
 * Version 2
 * (c) 2011 Sam Thompson <sam@websyntax.net>
 * The MIT License
 *****************************************************************************/

#ifndef ADRASTEA_HEADER
#define _ADRASTEA_H

#define IRC_LINE_END "\r\n"
#define CONFIG_FILE "bot.conf"

// Useful macros
#define streq(s1,s2) (strcmp(s1,s2)==0)
#define starts_with(s1,s2) (strncmp(s1,s2,strlen(s2))==0)
#define dot_so(fn) (strcmp(fn+strlen(fn)-3,".so")==0)

// For the modules
typedef void (*init_f) ();
typedef int (*resp_f) ();

typedef enum {
	FLAG_CONTINUE,
	FLAG_QUIT
} tick_flag;

typedef enum {
	EVENT_NONE,
	EVENT_MSG,
	EVENT_ACTION,
	EVENT_JOIN,
	EVENT_PART,
	EVENT_QUIT,
	EVENT_CHNICK,
	EVENT_NOTICE
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

int sock = 0;
tick_flag flag = FLAG_CONTINUE;

void error(const char * msg);
void errorf(const char * fmt, ...);
void interupt(int sig);
void _quit();
void quit();
int get_int_cfg(const char * name);
void get_str_cfg(const char * name, char * dest);

#endif
