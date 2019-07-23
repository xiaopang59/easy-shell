/*
 * 1)对于shell指令采用结构体存储，方便管道的切割与执行；
 * 2)采用strtok_r对于指令进行更合理的切割
 * 3)增加了一些简单的异常问题，包括文件路径问题，空行问题，指令错误等。
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "errHanding.h"
#include "PipeHanding.h"
#include "ExecuteInstruction.h"

int main(void)
{
	pid_t pid;
	char buf[LEN], copyBuf[LEN];
	int innerFlag;
	
	while (1) {
		printf("myshell$> ");
		fgets(buf, LEN, stdin);	// 读入shell指令
		
		if (buf[0] == '\n') continue;
		buf[strlen(buf) - 1] = '\0';
		
		strncpy(copyBuf, buf, sizeof(buf) - 1);

		innerFlag = ExecuteInnerInstruction(buf);	// 内置指令判断
		if (innerFlag == 0) {
			// 执行外部指令
			pid = fork();	// 建立新的进程
			if (pid == 0) {
				int stdinFlag = 0, stdinIndex = 0, stdoutFlag = 0, stdoutIndex = 0;
				
				SplitPipe(copyBuf);	// 管道的分割	

				// 判断是否存在输入重定向符号
				FindRedirectionMark(cmdv[0]->argvList, "<", &stdinFlag, &stdinIndex);
		
				// 存在输入重定向符号
				if (stdinFlag == 1) {
					char fileInName[LEN];
					cmdv[0]->argvList[stdinIndex] = NULL;
					strcpy(fileInName, cmdv[0]->argvList[stdinIndex + 1]);
					InputRedirection(fileInName);
				}


				// 判断是否存在输出重定向符号
				FindRedirectionMark(cmdv[num - 1]->argvList, ">", &stdoutFlag, &stdoutIndex);
		
				// 存在输出重定向符号
				if (stdoutFlag == 1) {
					char fileOutName[LEN];
					cmdv[num - 1]->argvList[stdoutIndex] = NULL;
					strcpy(fileOutName, cmdv[num - 1]->argvList[stdoutIndex + 1]);
					OutputRedirection(fileOutName);
				}

				
				ExecutePipeInstruction(0);	//执行外部指令
			}
			else if (pid > 0) 
				waitpid(pid, NULL, 0);
			else
				sys_err("fork", 5);
		}
	}	
	
	exit(0);
}
