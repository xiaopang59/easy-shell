#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "errHanding.h"

/*
 * Linux系统函数出错打印
 */
void sys_err(char *str, int errin)
{
	perror(str);
	exit(errin);
}
