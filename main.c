#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>


void split(char* cmd, char** params);
void execute(char** params);

int counter=0;
int flag=0;
FILE *file;

void main()
{
    char   cmd[100];
    char * parameter[10];
    char * user = getenv("USER");


    while (1){
        counter=0;
        flag=0;

        printf("\n%s@shell> ", user);

        // Read command from standard input
        if(fgets(cmd, sizeof(cmd), stdin) == NULL) break;

        // check & Remove newline character form end of command
        if(cmd[strlen(cmd)-1] == '\n') {
            cmd[strlen(cmd)-1] = '\0';
        }

        // Split on space cmd into array of parameters
        split(cmd, parameter);

        // check Exit
        if(strcmp(parameter[0],"exit") == 0) break;

        // for & at the end of cmd
        if (strcmp(parameter[counter-1],"&")==0){
            flag=1;
            parameter[counter-1]=NULL;
        }
        // Execute
        execute(parameter);



    }

}

void handler(int sig)
{
    //if (flag)flag=1;
    file = fopen("/home/mohamedessam/CLionProjects/untitled/logfile","a");
    pid_t pid;
// Split cmd into array of parameter
    if(pid==0){
        fprintf(file,"child terminate\nPid %d exited.\n", pid);
    }
    else fprintf(file,"Pid %d exited.\n", pid);
   // printf("Pid %d exited.\n", pid);
     fclose(file);
}

//split on space
void split(char* cmd, char** params){
    for(int i = 0; i < sizeof(params); i++) {
        params[i] = strsep(&cmd, " ");
        if(params[i] == NULL) break;
        counter++;
    }
}

//execution
void execute(char** params){

    signal(SIGCHLD,handler);
    // Fork process
    int pid = fork();

    // Error
    if (pid == -1) {
        printf("\nERROR happen\n");
    }

        // Child process
    else if (pid == 0) {
        // Execute command
        execvp(params[0],params);

        //if get error
        printf("\n%s: command not found\n",*params);
    }

        // Parent process
    else {
        // if condition for not wait if enter &
        if (!flag){

            // Wait for child process to finish
            wait(NULL);
            //printf("child terminate\n");

        }
    }
}


