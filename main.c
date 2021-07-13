#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#define MAX_LENGTH 10
#define MAX_CHAR 50
#define DELIMETER " "
#define clear() printf("\033[H\033[J")

int flag = 0;//flag of background process
char *log_file = "logFile.txt";
FILE * fptr;
void init();
void argsFormater(char line[]);
void scanner(char line[]);
int process(char* args[],char line[]);
int parser(char* args[], char line[]);
void signalHandler(int signal);
void execute(char* args[]);


/************************main************************/
int main() {
    //remove old log
    remove("logFile.txt");
    signal(SIGCHLD,signalHandler);
    char * args[MAX_LENGTH];
    char line[MAX_CHAR];
    init();
    while(parser(args,line)) {
        execute(args);
    }
    return 0;
}

/**********************init***********************/
void init(){
    clear();
    printf("\n\n\n\n******************"
        "************************");
    printf("\n\n\n\t****COCKLE SHELL****");
    printf("\n\n\n\n*******************"
        "***********************");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(2);
    clear();


}
/********************argsFormater*********************/
// modify format of input
void argsFormater(char line[]){
    int j = 0;
    int i = 0;
    // remove new line char \n
    while (line[i] != '\n'){
        i++;
    }
    line[i] = '\0';
    // checks for present &
    while(line[j] !='\0'){
        if(line[j]== '&'){
            flag = 1;
            line[j]='\0';
        }
        j++;
    }
}

/***********************scanner***********************/
// scan input from user
void scanner(char line[]) {
    // reads from console
    fgets(line, MAX_CHAR, stdin);
    argsFormater(line);

}

/***********************process***********************/
int process(char* args[],char line[]){
    int i = 0;
    args[i]=strtok(line,DELIMETER);
    // if input is null
    if(args[i]==NULL) {
        printf("Please Enter a COMMAND!\n");
        return 1;
    }
    while(strcmp(line,"exit")==0 ){exit(0);}
    while(args[i]!= NULL){
        args[++i]=strtok(NULL,DELIMETER);
    }
    return 1;
}

/***********************parser***********************/
int parser(char* args[], char line[]){
    //scan input
    char* username = getenv("USER");
    printf(username);
    printf(">> ");
    scanner(line);
    //scan array of chars
    process(args,line);
    return 1;
}

/********************signalHandler********************/

void signalHandler(int signal){
    if (signal==SIGCHLD) {
        fptr = fopen(log_file, "a");
        fputs("Child Process was Terminated\n", fptr);
        printf("child terminated \n");
        fclose(fptr);
    }
}

/***********************execute***********************/

void execute(char* args[]){
    int status;
    pid_t pid = fork();
    if (flag == 1 && pid != 0){
            //nothing
    } else {
        if (pid == 0) {
                //child
            execvp(args[0], args);
            printf("ERROR! invalid command \n");
        } else {
                //parent
            waitpid(pid,&status,WUNTRACED);
        }
    }
    flag = 0;
}


