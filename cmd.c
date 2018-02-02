#include "cmd.h"
//Your includes come here
void cmd_init(cmd *c)
{
    //the command originally inputed by the user
    c->init_cmd = NULL;

    //number of members
    c->nb_cmd_members = 0;

    //each position holds a command member
    c->cmd_members = NULL;

    //cmd_members_args[i][j] holds the jth argument of the ith member
    c->cmd_members_args = NULL;

    //number of arguments per member
    c->nb_members_args = NULL;

    //the path to the redirection file
    c->redirection = NULL;

    //the redirecction type (append vs. override)
    c->redirection_type = NULL;
}
//Prints the contents of members_args to the console
void print_members_args(cmd *c)
{
    //your implementation comes here
    int i, j;

    for(i = 0; i < c->nb_cmd_members; i++)
    {
        printf("number of arg is %d\n", c->nb_members_args[i]);
        for(j = 0; j < c->nb_members_args[i]; j++)
            printf("arg is %s\n", c->cmd_members_args[i][j]);
    }
}

//Frees the memory allocated to store member arguments
void free_members_args(cmd *c)
{
    //your implementation comes here
    int i = 0, j = 0;
    for(i = 0; i < c->nb_cmd_members; i++)
    {
        for(j = 0; j < c->nb_members_args[i]; j++)
        {
            free(c->cmd_members_args[i][j]);
        }
        free(c->cmd_members_args[i]);
    }
    free(c->nb_members_args);
    free(c->cmd_members_args);
    c->cmd_members_args = NULL;
}

//Prints the contents of members to the console
void print_members(cmd *c)
{
    //your implementation comes here
    int i;
    printf("number of member is %d\n", c->nb_cmd_members);
    for(i = 0; i < c->nb_cmd_members; i++)
    {
        printf("member is %s\n", c->cmd_members[i]);
    }
}

//Frees the memory allocated to store member information
void free_members(cmd *c)
{
    //your implementation comes here
    int i;

    for(i = 0; i < c->nb_cmd_members; i++)
    {
        free(c->cmd_members[i]);
    }
    c->nb_cmd_members = 0;
    free(c->cmd_members);
    c->cmd_members = NULL;
}

//Prints the redirection information for member i
void print_redirection(cmd *c, int i)
{
    //your implementation comes here
    int  j;
    for(j = 0; j < 3; j++)
        printf("redirection %d is %s\n", i, c->redirection[i][j]);
}

//Frees the memory allocated to store redirection info
void free_redirection(cmd *c)
{
    //your implementation comes here
    int i;
    for(i = 0; i < c->nb_cmd_members; i++)
    {
        free(c->redirection[i][0]);
        free(c->redirection[i][1]);
        free(c->redirection_type[i]);
    }
    free(c->redirection);
    free(c->redirection_type);
    c->redirection = NULL;
    c->redirection_type = NULL;
}

//Remplit les champs cmd_args et nb_args_membres
void parse_members_args(cmd *c)
{
    //your implementation comes here
    int i;
    c->cmd_members_args = (char ** *)malloc(c->nb_cmd_members * sizeof(char **));
    c->nb_members_args = (unsigned int *)malloc(c->nb_cmd_members * sizeof(int));
    for(i = 0; i < c->nb_cmd_members; i++)
    {
        c->nb_members_args[i] = 0;
    }
    for(i = 0; i < c->nb_cmd_members; i++)
    {
        c->cmd_members_args[i] = NULL;
        int j = 0;
        char *eachcmd = c->cmd_members[i];
        char *move = eachcmd;
        //printf("%s\n",move);
        while(*move != '\0')
        {
            if(*move == ' ' && (*eachcmd) != '<' && (*eachcmd) != '>' && (*eachcmd) != '2')
            {
                c->cmd_members_args[i] = (char **)realloc(c->cmd_members_args[i], (c->nb_members_args[i] + 1) * sizeof(char *));
                c->cmd_members_args[i][j] = subString(eachcmd, move);
                c->nb_members_args[i]++;
                eachcmd = move + 1;
                j++;
            }
            move++;
        }
        if(*eachcmd != '<' && *eachcmd != '>' && (*eachcmd) != '2')
        {
            c->cmd_members_args[i] = (char **)realloc(c->cmd_members_args[i], (c->nb_members_args[i] + 2) * sizeof(char *));
            c->cmd_members_args[i][j] = subString(eachcmd, move);
            c->nb_members_args[i]++;
            j++;
        }
        c->cmd_members_args[i][j] = NULL;
        c->nb_members_args[i]++;

    }
}

//Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s, cmd *c)
{
    //your implementation comes here
    int i = 0;
    char *start = s;//s is the end
    free(c->init_cmd);
    c->init_cmd = s;
    while(*s != '\0')
    {
        if(*s == '|')
        {
            c->cmd_members = (char **)realloc(c->cmd_members, (c->nb_cmd_members + 1) * sizeof(char *));
            // c->cmd_members[i] = (char*)malloc(sizeof(char));
            c->cmd_members[i] = subString(start, s);
            removeStrRTrim(c->cmd_members[i]);
            removeStrLTrim(c->cmd_members[i]);
            c->nb_cmd_members++;
            i++;
            s++;
            start = s;
        }
        else
        {
            s++;
        }
    }
    // free(c->cmd_members[i]);
    //printf("%s\n",*s);
    c->cmd_members = (char **)realloc(c->cmd_members, (c->nb_cmd_members + 1) * sizeof(char *));
    // c->cmd_members[i] = (char*)malloc(sizeof(char));
    c->cmd_members[i] = subString(start, s);
    removeStrRTrim(c->cmd_members[i]);
    removeStrLTrim(c->cmd_members[i]);
    c->nb_cmd_members++;

}

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c)
{
    //your implementation comes here

    c->redirection = (char ** *)realloc(c->redirection, (i + 1) * sizeof(char **));
    c->redirection_type = (int **)realloc(c->redirection_type, (i + 1) * sizeof(int *));
    c->redirection[i] = (char **)malloc(3 * sizeof(char *));
    c->redirection[i][STDIN] = NULL;
    c->redirection[i][STDOUT] = NULL;
    c->redirection[i][STDERR] = NULL;
    c->redirection_type[i] = (int *)malloc(2 * sizeof(int));
    char *eachcmd = c->cmd_members[i];
    char *move = eachcmd;
    while(*move != '\0')
    {

        if(*move == '<')
        {
            if(*(move + 1) == '<') // append
            {
                move += 3;
                eachcmd = move;
                c->redirection_type[i][STDIN] = APPEND;
            }
            else
            {
                move += 2;
                eachcmd = move;
                c->redirection_type[i][STDIN] = OVERRIDE;
            }
            while(*move != ' ' && *move != '\0')
            {
                move++;
            }

            c->redirection[i][STDIN] = subString(eachcmd, move);
            removeStrRTrim(c->redirection[i][STDIN]);
            removeStrLTrim(c->redirection[i][STDIN]);
            // eachcmd = move + 1;
        }

        if(*move == '>')
        {

            if(*(move + 1) == '>') // append
            {
                move += 3;
                eachcmd = move;
                c->redirection_type[i][STDOUT] = APPEND;

            }
            else
            {
                move += 2;
                eachcmd = move;
                c->redirection_type[i][STDOUT] = OVERRIDE;
            }
            while(*move != ' ' && *move != '\0')
            {
                move++;
            }
            c->redirection[i][STDOUT] = subString(eachcmd, move);
            removeStrRTrim(c->redirection[i][STDOUT]);
            removeStrLTrim(c->redirection[i][STDOUT]);
            //eachcmd = move + 1;
        }

        if(*move == '2' && *(move + 1) == '>')
        {
            move++;
            if(*(move + 1) == '>') // append
            {
                move += 3;
                eachcmd = move;
                c->redirection_type[i][STDERR] = APPEND;

            }
            else
            {
                move += 2;
                eachcmd = move;
                c->redirection_type[i][STDERR] = OVERRIDE;
            }
            while(*move != ' ' && *move != '\0')
            {
                move++;
            }
            c->redirection[i][STDERR] = subString(eachcmd, move);
            removeStrRTrim(c->redirection[i][STDERR]);
            removeStrLTrim(c->redirection[i][STDERR]);
            //eachcmd = move + 1;
        }
        move++;
    }
}
