#ifndef EXECUTE_INSTRUCTION_H
#define EXECUTE_INSTRUCTION_H

void FreeMalloc(char *argvList[], int index);
void ExecGetcwd(char *errInfo);
int ExecuteInnerInstruction(char *argvList[]);
int InputRedirection(char *fileName);
int OutputRedirection(char *fileName);
void ExecuteOuterInstruction(char *argvList[]);
void InputOutputRedirection(int FILENO, int redirectionFd);
void FindRedirectionMark(char *argvList[], char *redirectionSymbol, int *stdinFlag, int *stdinIndex, int num);

#endif

