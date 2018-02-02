#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "shell_fct.h"
#include "auto.h"
//To complete
int main(int argc, char **argv)
{
    //..........
    int ret = MYSHELL_CMD_OK;
    char *readlineptr;
    struct passwd *infos;
    char str[1024];
    char hostname[256];
    char workingdirectory[256];
    int status;
    cmd c;
    cmd_init(&c);

    //..........
    while(ret != MYSHELL_FCT_EXIT)
    {
        //Get your session info
        rl_attempted_completion_function = my_completion;
        infos = getpwuid(getuid());
        gethostname(hostname, 256);
        getcwd(workingdirectory, 256);
        //Print it to the console
        printf("***************************************************************************\n");
        printf("* Attention: The auto completion function is only available in CODEBLOCKS *\n");
        printf("* If you press tab in terminal, the program may be blocked!!!!            *\n");
        printf("***************************************************************************\n");
        sprintf(str, "\n{myshell}%s@%s:%s$ ", infos->pw_name, hostname, workingdirectory);
        readlineptr = readline(str);
        if(readlineptr && *readlineptr)
        {
            rl_bind_key('\t',rl_complete);
            pid_t pid;
            add_history(readlineptr);
            parse_members(readlineptr, &c);
            printf("init cmd = %s\n", c.init_cmd);
            print_members(&c);
            printf("------------\n");
            parse_members_args(&c);
            print_members_args(&c);
            if(strcmp(c.cmd_members[0], "exit") == 0)
            {
                exit(MYSHELL_FCT_EXIT);
            }
            int i = 0;
            for(i = 0; i < c.nb_cmd_members; i++)
            {
                parse_redirection(i,&c);
                print_redirection(&c,i);
            }
            pid = fork();
            if(pid==0){
                int res;
                res = exec_command(&c);
                exit(res);
            }
            wait(&status);
            free_redirection(&c);
            free_members_args(&c);
            free_members(&c);

        }

        //readlineptr = NULL;
    }
    //..........
    return 0;
}
