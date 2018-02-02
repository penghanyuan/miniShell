#include "cmd.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
//Your imports come here

//Terminate shell
#define MYSHELL_FCT_EXIT 1


typedef struct exection{
    pid_t *pids;
    int **tubes;
    unsigned int exec_count;
}exec;
//Execute a command
int exec_command(cmd *c);
void exec_init(exec *e);
