#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

char error_message[] = "An error has occurred\n"; 

void myPrint(char *msg)
{
    write(STDOUT_FILENO, msg, strlen(msg));
}

int loadRedirect(char* cmd)
{
    int clength = strlen(cmd);
    int redirects = 0; 
    int Rvalue = -1;

    int i;
    for (i=0; i < clength; i++) {
        if (cmd[i] == '>'){
            redirects++;
        }
    }
    if (redirects > 1){
        return Rvalue;
    }
    if(redirects == 1){
        for (i=0; i < clength; i++) {
            if (cmd[i+1]!='+'){
                Rvalue = 1;
            }
        }
        for (i=0; i < clength-1; i++){
            if (cmd[i+1]=='+'){
                Rvalue = 2;
            }
        }
    }

    if (Rvalue == -1){
        Rvalue = 0;
    }
    return Rvalue;
}

int parseWithoutRedirect(char **arg, char* noRedirect)
{
    char* noRedirect2 = strdup(noRedirect);
    char *tok = strtok(noRedirect2, " \n\t");

    int i;
    for(i = 0; tok != NULL && i < 512; i++){
        arg[i] = strdup(tok);
        tok = strtok(NULL, " \n\t");
    }
    arg[i] = NULL; 

    return ++i;
}

char* getPath()
{
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    return getcwd(cwd, sizeof(cwd));
  } else {
    return getcwd(cwd, sizeof(cwd)*4);
  }
}

int parseComands(char **arg, char* cmd, char **f)
{
    int redirect = loadRedirect(cmd);
    int argc = -1;
    char* cmd2 = strdup(cmd);
    char* before;
    char* after;
    char* after1;
    char* after2;

    switch(redirect) {
        case 0 :
            argc = parseWithoutRedirect(arg, cmd);
            f = NULL;
            break;
        case -1 :
            write(STDOUT_FILENO, error_message, strlen(error_message));
            break;
        case 1 :
            if (cmd[0] == '>'){
                before = NULL;
                after = strtok(cmd2, ">\n\t");
            } else {
                before = strtok(cmd2, ">\n\t");
                after = strtok(NULL, ">\n\t");
            }
            break;
        case 2 :
            if (cmd[0] == '>' && cmd[1] == '+'){
                before = NULL;
                after = strtok(cmd2, ">+\n\t");
            } else {
                before = strtok(cmd2, ">+\n\t");
                after = strtok(NULL, ">+\n\t");
            }
    }

    if(redirect == 1 || redirect == 2){
        if (after == NULL){
            write(STDOUT_FILENO, error_message, strlen(error_message));
        } else {
            after1 = strtok(after, " \n\t");
            after2 = strtok(NULL, " \n\t");
            if ((after1 == NULL) || after2){
                write(STDOUT_FILENO, error_message, strlen(error_message));;
            } else {
                f[0] = strdup(after1);
                argc = parseWithoutRedirect(arg, before);
            }
        }
    }

    return argc;
}

void advanced(char** arg, char** f)
{
    FILE* f2;
    FILE* t1;
    f2 = fopen(f[0], "r");
    t1 = fopen("t1", "w+");
    char buff[512];
    if(f2){
        while (fgets(buff, 512, f2)){
            if (fputs(buff, t1) < 0){
                write(STDOUT_FILENO, error_message, strlen(error_message));
            }
        }
        fclose(f2);
        remove(f[0]);
    }

    fclose(t1);
    int forkret;
    if ((forkret = fork()) == 0){
        int t2 = open("t2", O_RDWR|O_EXCL|O_CREAT, 0600);
        dup2(t2, 1);
    if (execvp(arg[0], arg) == -1){
        write(STDOUT_FILENO, error_message, strlen(error_message));
        exit(0);
    }
        close(t2);
        exit(0);
    } else {
        wait(NULL);
    }
    int nFileDescriptor = open(f[0], O_RDWR|O_EXCL|O_CREAT, 0600);
    if (nFileDescriptor < 0) {
        write(STDOUT_FILENO, error_message, strlen(error_message));
        close(nFileDescriptor);
        exit(0);
    }
    close(nFileDescriptor);

    FILE* nf = fopen(f[0], "w+");
    FILE* t1file = fopen("t1", "r");

    if (t1file == NULL) {
        write(STDOUT_FILENO, error_message, strlen(error_message));
    } else {
        while(fgets(buff, 512, t1file)) {
            fputs(buff, nf);
        }
    }

    FILE* t2file = fopen("t2", "r");       
    if (t2file == NULL) {
        write(STDOUT_FILENO, error_message, strlen(error_message));
    } else {
        while(fgets(buff, 512, t2file)) {
            fputs(buff, nf);
        }
    }
    remove("t1");
    remove("t2");
    exit(0);
}

void runCommands(char** arg, char* cmd, int argc, char** f)
{
    if (argc == -1 || argc == 1){
        return;
    } 

    char* arg1 = arg[0];
    char* arg2 = arg[1];
    int redirect = loadRedirect(cmd);

    if (strcmp(arg1, "exit") == 0){
        if (arg2 == NULL && redirect == 0)
            exit(0);
        else{
            write(STDOUT_FILENO, error_message, strlen(error_message));
            return;
        }
    } else if (strcmp(arg1, "cd") == 0){
        if (argc == 2 && redirect == 0){
            chdir(getenv("HOME"));
        }
        else if (argc == 3 && redirect == 0){
            int c = chdir(arg2);
            if(c){
                write(STDOUT_FILENO, error_message, strlen(error_message));
            }
        } else {
            write(STDOUT_FILENO, error_message, strlen(error_message));
            return;
        }
    }else if (strcmp(arg1, "pwd") == 0){
        if (arg2 == NULL && redirect == 0){
            char *path;
            path = getPath();
            myPrint(strcat(path, "\n"));
        } else {
            write(STDOUT_FILENO, error_message, strlen(error_message));
            return;
        }
    } else{
        int forkret;
        int filedecriptor;

        if ((forkret = fork()) == 0){
            if (redirect == 0){
                if (execvp(arg[0], arg) == -1){
                    write(STDOUT_FILENO, error_message, strlen(error_message));
                    exit(0); 
                }
            }else if(redirect == 1){
                filedecriptor = open(f[0], O_WRONLY|O_TRUNC|O_EXCL|O_CREAT,
                S_IRUSR|S_IRGRP|S_IWGRP|S_IWUSR);
                if (filedecriptor != -1){
                    dup2(filedecriptor, 1);
                    close(filedecriptor);
                } else {
                    write(STDOUT_FILENO, error_message, strlen(error_message));
                    exit(0);
                }
                if (execvp(arg[0], arg) == -1){
                    write(STDOUT_FILENO, error_message, strlen(error_message));
                    exit(0);
                }
            }else if(redirect == 2){
                advanced(arg, f);
            }
        } else{
            wait(NULL);
        }
    }
}

int main(int argc, char *arg[])
{
    char* cmd[512];
    int num_cmds;
    char* argCmd[512];
    int argcCmds;

    char buffB[1024];
    char buff[1024];
    char* f[1];
    char* input;
    FILE* batchf = fopen(arg[1], "r");

    switch(argc){
        case 1 :
            while (1) {
                myPrint("myshell> ");
                input = fgets(buff, 1024, stdin);
                if (!input){
                    exit(0);
                }
                if (strlen(buff) > 513){
                    myPrint(buff);
                    write(STDOUT_FILENO, error_message, strlen(error_message));
                    continue;
                } else {
                    int n = 0;
                    char *tok = strtok(buff, ";\n");
                    for(n = 0; tok != NULL && n < 512; n++){
                        cmd[n] = strdup(tok);
                        tok = strtok(NULL, ";\n");
                    }
                    num_cmds = n;                   
                    int i = 0;
                    while (i < num_cmds){
                        argcCmds = parseComands(argCmd, cmd[i], f);
                        runCommands(argCmd, cmd[i], argcCmds, f);
                        i++;
                    }
                }
            }
            break;
        case 2 :
            if (batchf == NULL){
                write(STDOUT_FILENO, error_message, strlen(error_message));
                exit(0);
            }
            while(fgets(buffB, 1024, batchf) != NULL){
                char *line = strdup(buffB);
                char *legit = strtok(line, " \n\t");

                int l = strlen(buffB);
                if (legit == NULL){
                    continue;
                } else if (legit != NULL && l > (513)){
                    myPrint(buffB);
                    write(STDOUT_FILENO, error_message, strlen(error_message));
                    continue;
                } else {
                    myPrint(buffB);
                    int n = 0;
                    char *tok = strtok(buffB, ";\n");
                    for(n = 0; tok != NULL && n < 512; n++){
                        cmd[n] = strdup(tok);
                        tok = strtok(NULL, ";\n");
                    }
                    num_cmds = n;
                    int i = 0;
                    while(i < num_cmds){
                        remove("t1");
                        remove("t2");
                        argcCmds = parseComands(argCmd, cmd[i], f);
                        runCommands(argCmd, cmd[i], argcCmds, f);
                        i++;
                    }
                }
            }
            break;
        default:
            write(STDOUT_FILENO, error_message, strlen(error_message));
            exit(0);
    }
}