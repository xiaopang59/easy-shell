#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GetInstruction.h"

/*
 * 释放argvList指针空间
 */
void FreeMalloc(char *argvList[], int index)
{
	while (argvList[index] != NULL) {
		free(argvList[index]);		
		argvList[index++] = NULL;
	}
}

/*
 * 将字符串传入参数表内
 */
void GetString(char **argv, char *buf)
{
	char *buffer;

	buffer = malloc(strlen(buf) + 1);
	if (buffer == NULL) {
		fprintf(stderr, "malloc error\n");
		exit(1);
	}

	strcpy(buffer, buf);
	*argv = buffer;
}

/*
 *  对于字符串进行分割
 */
int StringSplit(char *buf, char *argvList[])
{
	int i, j, num, last;
	char buffer[LEN];

	//释放argvList的空间
	FreeMalloc(argvList, 0);
	
	num = 0;
	i = 0;
	
	while (num < MAX) {
		if (buf[i] == '\n') {
			argvList[num] = NULL;
			return num;	
		}
		
		if (buf[i] == ' ') 
			i++;
		last = i;
		
		while (buf[i] != ' ' && buf[i] != '\n')
			i++;
		
		for (j = last; j < i; j++)
			buffer[j - last] = buf[j];
		
		buffer[j - last] = '\0';
		GetString(&argvList[num++], buffer);
	}	

	return -1;
}
