#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>

char ** get_input(char * input){
    char ** command = malloc(8 * sizeof(char*));
    char * seperator = " ";
    char * parsed;
    int index = 0;

    parsed = strtok(input, seperator);
    while (parsed != NULL){
        command[index] = parsed;
        index++;

        parsed = strtok(NULL,seperator);
    }

    command[index] = NULL;
    return command;
}

int cd(char * path){
    return chdir(path);
}

int main(){
    char **command;
    char *input;
    int stat_loc;

    while (1){
        input = readline("my_shell> ");
        command = get_input(input);

        if (strcmp(command[0],"cd") == 0){
            if (cd(command[1]) < 0){
                perror(command[1]);
            }
            continue;
        }

        pid_t child_pid = fork();
        if (child_pid < 0){
            perror("fork() failed!");
            exit(1);
        }
        if (child_pid == 0){
            if (execvp(command[0],command) < 0){
                perror(command[0]);
                exit(1);
            }
            printf("This won't be printed if execvp is successul\n");
        }
        else{
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }

        free(input);
        free(command);
    }

    return 0;
}