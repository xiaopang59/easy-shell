#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "errHanding.h"
#include "GetInstruction.h"
#include "ExecuteInstruction.h"

int main(void)
{
	int i;
	pid_t pid;
	char buf[LEN];
	char *argvList[MAX];	// shell指令参数表
	int num;		// shell指令参数个数
	int innerFlag;
	
	while (1) {
		printf("myshell$> ");
		fgets(buf, LEN, stdin);	// 读入单行指令
		
		num = StringSplit(buf, argvList);	// 指令分割
		if (num == -1) {
			fprintf(stderr, "input parameter is too long");
			exit(1);
		}
		
		innerFlag = ExecuteInnerInstruction(argvList);	// 内置指令判断
		if (innerFlag == 0) {
			// 执行外部指令
			pid = fork();	// 建立新的进程
			if (pid == 0) {
				int stdinFlag = 0, stdinIndex = 0, stdoutFlag = 0, stdoutIndex = 0;
				
				// 判断是否存在输入重定向符号
				FindRedirectionMark(argvList, "<", &stdinFlag, &stdinIndex, num);
		
				// 存在输入重定向符号
				if (stdinFlag == 1) {
					char fileInName[LEN];
					strcpy(fileInName, argvList[stdinIndex + 1]);
					InputRedirection(fileInName);
				}

				// 判断是否存在输出重定向符号
				FindRedirectionMark(argvList, ">", &stdoutFlag, &stdoutIndex, num);
		
				// 存在输出重定向符号
				if (stdoutFlag == 1) {
					char fileOutName[LEN];
					strcpy(fileOutName, argvList[stdoutIndex + 1]);
					OutputRedirection(fileOutName);
				}
			
				// 释放argvList的内存空间
				if ((stdinIndex == 0 && stdoutIndex != 0) || (stdinIndex != 0 && stdoutIndex == 0)) {
					i = stdinIndex == 0 ? stdoutIndex : stdinIndex;
					FreeMalloc(argvList, i);
				}
				else if (stdinIndex != 0 && stdoutIndex != 0) {
					i = stdinIndex < stdoutIndex ? stdinIndex : stdoutIndex;
					FreeMalloc(argvList, i);
				}
				
				ExecuteOuterInstruction(argvList);	//执行外部指令
			}
			else if (pid > 0) 
				waitpid(pid, NULL, 0);
			else
				sys_err("fork", 5);
		}
	}	
	
	exit(0);
}



