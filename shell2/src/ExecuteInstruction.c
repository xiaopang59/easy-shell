#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "errHanding.h"
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
int ExecuteInnerInstruction(char *buf)
{
	char  *argvList[MAX];
	char *copyBuf;
	char errBuf[LEN];
	char *argv;
	int i = 0;
	
	copyBuf = buf;	

	while ((argv = strtok_r(copyBuf, " ", &copyBuf)) != NULL) {
		argvList[i] = argv;
		i++;	//记录命令条数
	}
	argvList[i] = NULL;

	if (strcmp(argvList[0], "exit") == 0) {
		// exit
		exit(0);	
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
void InputRedirection(char *fileName)
{
	int fd;
	
	fd = open(fileName, O_RDONLY);
	if (fd == -1)
		sys_err("open", 4);
	
	dup2(fd, STDIN_FILENO);
	close(fd);
}


/*
 * 输出重定向
 */
void OutputRedirection(char *fileName)
{
	int fd;

	fd = open(fileName, O_RDWR | O_CREAT, 0777);
	if (fd == -1)
		sys_err("open", 4);

	dup2(fd, STDOUT_FILENO);
	close(fd);
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
 * 执行管道命令
 */
void ExecutePipeInstruction(int index)
{
	if (index == num - 1)
		ExecuteOuterInstruction(cmdv[index]->argvList);
	
	int fd[2];
	pid_t pid;	
	
	if (pipe(fd) < 0) {
		//创建管道，0读，1写	
		sys_err("pipe", 6);
	}
	
	if ((pid = fork()) == 0) {
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		ExecuteOuterInstruction(cmdv[index]->argvList);
	}	
	else if (pid > 0) {
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		ExecutePipeInstruction(index + 1);	
	}
	else	
		sys_err("fork", 7);
}

/*
 * 寻找是否存在输入/输出重定向符
 */
void FindRedirectionMark(char *argvList[], char *redirectionSymbol, int *stdinFlag, int *stdinIndex)
{
	int i = 0;
	while (argvList[i] != NULL) {
		if (strcmp(argvList[i++], redirectionSymbol) == 0) {
			*stdinFlag = 1;
			*stdinIndex = i - 1;
			break;
		}
	}
}
