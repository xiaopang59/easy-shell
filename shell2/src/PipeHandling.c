#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PipeHanding.h"
#include "ExecuteInstruction.h"

/*
 * 字符串去掉前后空格
 * */
void StringTrimSpace(char **buf)
{
	int i, j;
	char *copyBuf;
	
	copyBuf = (char *)malloc(strlen(*buf) + 1);
	strcpy(copyBuf, *buf);
	
	if (copyBuf == NULL) {
		printf("malloc error!");
		exit(0);	
	} 

	i = 0;
	j = strlen(copyBuf) - 1;
	
	while (copyBuf[i] == ' ')
		++i;
	while (copyBuf[j] == ' ')
		--j;
	
	memset(*buf, 0, strlen(*buf));
	strncpy(*buf, copyBuf + i, j - i + 1);

	if (copyBuf != NULL) {
		free(copyBuf);
		copyBuf = NULL;	
	}
}

/*
 * 切分单个管道
 * */
void SplitCmd(char *buf)
{
	struct cmd_list *cmd = (struct cmd_list*)malloc(sizeof(struct cmd_list));
	char *copyBuf, *argv;
	//int i;
	
	cmdv[num++] = cmd;
	cmd->argc = 0;
	copyBuf = buf;
	
	while ((argv = strtok_r(copyBuf, " ", &copyBuf)) != NULL)
		cmd->argvList[cmd->argc++] = argv;
	cmd->argvList[cmd->argc] = NULL;
}

/*
 * 切分管道
 * */
void SplitPipe(char *buf)
{
	char *argv;
	char  *copyBuf = buf;
	
	while ((argv = strtok_r(copyBuf, "|", &copyBuf)) != NULL) {
		StringTrimSpace(&argv);
		//printf("%s\n", argv);
		SplitCmd(argv);
	}
}
