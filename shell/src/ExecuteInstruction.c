#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "errHanding.h"
#include "GetInstruction.h"
#include "ExecuteInstruction.h"

/*
 * 执行getpwd函数
 */
void ExecGetcwd(char *errInfo)
{
	char buf[LEN];
	
	strcat(errInfo, " getpwd");	

	if (getcwd(buf, sizeof(buf)) != NULL) {
		printf("Current dir is: %s\n", buf);
		return;
	}
	
	sys_err(errInfo, 2);
}

/*
 * 执行内置指令
 */
int ExecuteInnerInstruction(char *argvList[])
{
	char errBuf[LEN];

	if (strcmp(argvList[0], "exit") == 0) {
		// exit
		exit(0);	
		return 1;
	}
	else if (strcmp(argvList[0], "pwd") == 0){
		// pwd
		strcpy(errBuf, "pwd");
		ExecGetcwd(errBuf);
		return 1;
	}
	else if (strcmp(argvList[0], "cd") == 0) {
		// cd
		if (chdir(argvList[1]) != -1) {
			strcpy(errBuf, "cd");
			ExecGetcwd(errBuf);
		}
		else 
			sys_err("chdir", 3);
	
		return 1;
	}
	
	return 0;
}

/*
 * 输入重定向
 */
int InputRedirection(char *fileName)
{
	int fd, saveInputfd = 0;
	
	fd = open(fileName, O_RDONLY);
	if (fd == -1)
		sys_err("open", 4);
	
	saveInputfd = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return saveInputfd;
}

/*
 * 输出重定向
 */
int OutputRedirection(char *fileName)
{
	int fd, saveOutputfd;

	fd = open(fileName, O_RDWR | O_CREAT, 0777);
	if (fd == -1)
		sys_err("open", 4);
	
	saveOutputfd = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return saveOutputfd;
}

/*
 * 执行外部命令
 */
void ExecuteOuterInstruction(char *argvList[])
{
	int error;
	error = execvp(argvList[0], argvList);
	if (error == -1)
		sys_err("execvp", 5);
}


/*
 * 将输入和输出重新定向到原标识符
 */
void InputOutputRedirection(int FILENO, int redirectionFd)
{
	dup2(FILENO, redirectionFd);
	close(redirectionFd);
}

/*
 * 寻找是否存在输入/输出重定向符
 */
void FindRedirectionMark(char *argvList[], char *redirectionSymbol, int *stdinFlag, int *stdinIndex, int num)
{
	int i = 0;
	while (i < num) {
		if (strcmp(argvList[i++], redirectionSymbol) == 0) {
			*stdinFlag = 1;
			*stdinIndex = i - 1;
			break;
		}
	}
}
