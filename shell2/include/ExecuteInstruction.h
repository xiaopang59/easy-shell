#ifndef EXECUTE_INSTRUCTION_H
#define EXECUTE_INSTRUCTION_H

#define LEN 100		
#define MAX 100

//shell指令单个管道结构体
struct cmd_list{
	int argc;	//单个管道参数个数
	char *argvList[MAX];
};

struct cmd_list *cmdv[MAX];	//shell指令
int num;			//shell管道个数

void ExecGetcwd(char *errInfo);
int ExecuteInnerInstruction(char *buf);
void InputRedirection(char *fileName);
void OutputRedirection(char *fileName);
void ExecuteOuterInstruction(char *argvList[]);
void ExecutePipeInstruction(int index);
void FindRedirectionMark(char *argvList[], char *redirectionSymbol, int *stdinFlag, int *stdinIndex);

#endif

