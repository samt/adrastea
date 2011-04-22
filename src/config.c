/******************************************************************************
 * Adrastea IRC Bot
 *
 * Version 2
 * (c) 2011 Sam Thompson <sam@websyntax.net>
 * The MIT License
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cfgs {
	char name[51];
	char value[204];
};

struct cfgs configs[30];
int configsize = 0;
void readline(FILE *, char *, int);

/*
 * Load Config
 * @return void
 */
void load_config()
{
	char line[255], lvalue[51], rvalue[204];
	char flag, rvs;
	int i, j, k;
	FILE * fp;

	fp = fopen(CONFIG_FILE, "r");

	if (fp == NULL)
	{
		error("The configuration file could not be opened for reading");
	}

	k = 0;
	do
	{
		readline(fp, line, sizeof(line));

		flag = 'l';
		rvs = 'n';
		j = 0;

		for(i = 0; i < sizeof(line) && line[i] != EOF && line[i] != '\0' && !(line[i] == '#' && flag == 'l'); i++)
		{
			if(line[i] == '#' && flag == 'l')
			{
				break;
			}

			if(line[i] == ':' && flag != 'r')
			{
				flag = 'r';
				lvalue[j + 1] = '\0';
				j = 0;

				continue;
			}

			if((line[i] == ' ' || line[i] == '\t') && rvs == 'n')
			{
				continue;
			}

			if (flag == 'l')
			{
				lvalue[j] = line[i];
			}
			else
			{
				rvs = 'y';
				rvalue[j] = line[i];
			}

			j++;
		}

		if (strlen(lvalue) > 0 && strlen(rvalue) > 0)
		{
			strcpy(configs[k].name, lvalue);
			strcpy(configs[k].value, rvalue);

			k++;
		}
		
		memset(lvalue, '\0', 51);
		memset(rvalue, '\0', 204);
	}
	while(line[0] != EOF);

	configsize = k;
}

/*
 * Get a string config value
 * @param char ptr - Name of config
 * @param char ptr - Destination
 */
void get_str_cfg(const char * name, char * dest)
{
	int i;
	for (i = 0; strcmp(name, configs[i].name) != 0 && i < configsize; i++);

	strcpy(dest, configs[i].value);
}

/*
 * Get an integer config value
 * @param char ptr - Name of config
 * @return int - Value 
 */
int get_int_cfg(const char * name)
{
	int i;
	for (i = 0; strcmp(name, configs[i].name) != 0 && i < configsize; i++);
	return (configsize == i) ? 0 : atoi(configs[i].value);
}

/*
 * Read one line of the file and strip out all line enders
 * @param FILE pt - pointer to the file
 * @param char ptr - Line to store 
 * @param int - max
 */
void readline(FILE * fp, char * line, int max)
{
	char b;
	int i, offset = 0;

	for(i = 0; i < max; i++)
	{
		b = fgetc(fp);

		if(b == '\n')
		{
			line[i - offset] = '\0';
			break;
		}

		if(b == '\r')
		{
			offset++;
			continue;
		}

		line[i - offset] = b;
	}
}
