#include "shell_fct.h"


void exec_init(exec *e)
{
    e->pids = NULL;
    e->tubes = NULL;
    e->exec_count = 0;
}
void free_exec(exec *e)
{
    int i = 0;
    for(i = 0; i < e->exec_count; i++)
    {
        free(e->tubes[i]);
    }
    free(e->tubes);
    free(e->pids);
    e->tubes = NULL;
    e->pids = NULL;
    e->exec_count = 0;

}
exec e;
void create_pro(int i, cmd *my_cmd)
{
    e.tubes[i] = (int *)malloc(2 * sizeof(int));
    pipe(e.tubes[i]);
    e.pids[i] = fork();
    if(e.pids[i] == 0)
    {
        if(my_cmd->redirection[i][STDERR] != NULL)
        {
            //printf("SSS%d\n", my_cmd->redirection_type[i-1][STDERR]);
            int fderr = open(my_cmd->redirection[i][STDERR], O_RDWR | (my_cmd->redirection_type[i][STDERR]==0?O_APPEND:O_CREAT), S_IRWXU);
            dup2(fderr, 2);
            close(fderr);
        }
        if(i == 0) //第一个进程
        {
            if(my_cmd->redirection[i][STDIN] != NULL)
            {
                int fd = open(my_cmd->redirection[i][STDIN], O_RDWR | O_CREAT, S_IRWXU);
                dup2(fd, 0);
                close(fd);
            }
        }
        else
        {
            //Redirect input
            /*close all pipes that this process doesn't need*/
            int k;
            for(k = 0; k <= i - 2; k++)
            {
                close(e.tubes[k][0]);
                close(e.tubes[k][1]);
            }
            /*end*/
            close(e.tubes[i - 1][1]);
            dup2(e.tubes[i - 1][0], 0);
            close(e.tubes[i - 1][0]);

            //Redirect output

        }
        close(e.tubes[i][0]);//关闭管道的输入,因为子进程只输出
        dup2(e.tubes[i][1], 1);
        close(e.tubes[i][1]);

        waitpid(e.pids[i - 1], NULL, 0);
        //sleep(10);

        if(execvp(my_cmd->cmd_members_args[i][0], my_cmd->cmd_members_args[i]) == -1)
        {
            perror("execvp");
            exit(errno);
        }

        // exit(1);
    }
    else
    {
        printf("now is parent, pid is %d\n", getpid());
    }
}
void alarmHandler(int sigNum)
{
    if(e.pids != NULL)
    {
        printf("More than 5 seconds,stop the command\n");
        int i;
        for(i = 0; i < e.exec_count; i++)
            kill(e.pids[i], SIGKILL);
        exit(0);
    }

}

int exec_command(cmd *my_cmd)
{
    //Your implementation comes here
    int i = 0;
    char buffer[50];
    exec_init(&e);
    e.exec_count = my_cmd->nb_cmd_members;
    e.pids = (int *)malloc(e.exec_count * sizeof(int));
    e.tubes = (int **)malloc(e.exec_count * sizeof(int *));

    for(i = 0; i < e.exec_count; i++)
    {
        create_pro(i, my_cmd);
    }

    /* back to parent process*/
    /*close all pipes that parent doesn't need*/
    if(i > 1)
    {
        int k;
        for(k = 0; k <= i - 2; k++)
        {
            close(e.tubes[k][0]);
            close(e.tubes[k][1]);
        }

    }
    /*end*/
    //重定向输出
    if(my_cmd->redirection[i - 1][STDOUT] != NULL)
    {
        //printf("SSS%d\n", my_cmd->redirection_type[i-1][STDOUT]);
        int fdout = open(my_cmd->redirection[i - 1][STDOUT], O_RDWR | (my_cmd->redirection_type[i-1][STDOUT]==0?O_APPEND:O_CREAT), S_IRWXU);
        dup2(fdout, 1);
        close(fdout);
    }

    /*redirect error*/
    if(my_cmd->redirection[i - 1][STDERR] != NULL)
    {
        //printf("SSS%d\n", my_cmd->redirection_type[i-1][STDERR]);
        int fderr = open(my_cmd->redirection[i - 1][STDERR], O_RDWR | (my_cmd->redirection_type[i-1][STDERR]==0?O_APPEND:O_CREAT), S_IRWXU);
        dup2(fderr, 2);
        close(fderr);
    }


    /*Redirect input*/
    close(e.tubes[i - 1][1]);
    dup2(e.tubes[i - 1][0], 0);
    close(e.tubes[i - 1][0]);

    /* set alarm*/
    signal(SIGALRM, alarmHandler);
    alarm(5);

    /* read from pipe*/
    int j = 0;
    for(j = 0; j < e.exec_count; j++)
    {
        waitpid(e.pids[j], NULL, 0);
    }
    fflush(stdout);
    fflush(stdin);
    memset(buffer, '\0', 20);
    //printf("ready to print\n");
    while(fgets(buffer, 20, stdin) != NULL)
    {
        //printf("now is buffer\n");
        printf("%s", buffer);
    }
    /*reset*/
    free_exec(&e);
    exit(MYSHELL_FCT_EXIT);

    return 0; //your return code goes here
}
