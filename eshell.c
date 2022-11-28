#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <errno.h>

#define TEST_BUFFER_SIZE 100



void command_initialiser (char **args);

void exitfunc ();
void echo (char **args);
void PWD ();
void cd (char **args);

void execute(char **args){
	int pid;
	pid = fork();
	if (pid == 0){

		if (execvp(args[0],args) == -1){
			printf("command doesn't exist\n");
		}

	}
	else{
		wait(&pid);
	}
}

char** tok(char* line){	
	char** ret = (char**)malloc(sizeof(char*)*TEST_BUFFER_SIZE);

	int i = 0;

	
	char* toke;
    char ch = 'a';

	toke = strtok(line," \n\t\r\"");

	while(toke != NULL){
		ret[i] = toke;
        // printf("%s ",ret[i]);
		i++;
		toke = strtok(NULL," \n\t\r\"");
	}
    // ret[i] = "/home";
    // i++;
	ret[i]=NULL;
	return ret;
	
}

void simple_shell(){
	char* commands;                          //for taking commands as string and store it
	char** parsedcommads;                    //for making an array of strings to divide single command into main command and its arguments
	
	commands = (char *)malloc(sizeof(char)*TEST_BUFFER_SIZE);
	size_t n = 0;
	ssize_t noc;

    printf("simpleshell>>");

	noc = getline(&commands,&n,stdin);         // noc stores no of character read by getline fuction
	
	if (noc == -1) {
        if (feof(stdin)) 
           		exit(1);        	
        else {
           		perror("read");
           		exit(0);
        }
    }

	parsedcommads = tok(commands);	
    // for (int i =0;parsedcommads[i]!=NULL;i++){
    //     printf("%s ", parsedcommads[i]);
    // }
	command_initialiser(parsedcommads);
	
	free(commands);
	free(parsedcommads);
    	
}


int main(){
	
	while(1){
		
		simple_shell();
	
	}
}

void exitfunc(){
	exit(0);
}

void echo(char **args){
	int i;
	for (i=1;args[i]!=NULL;i++){
		printf("%s ",args[i]);
	}
	printf("\n");
}

void PWD (){
	char buf[1024];

	if (getcwd(buf,1024) == NULL){
		printf("Getting current working directory failed!");
		if (errno == ERANGE){
			printf("Path excceeds the max limit");
		}
	}
	else{
		printf("%s\n",buf);
	}
}

void cd (char **args){
	if (args[1] == NULL){
		if (chdir(getenv("HOME")) == -1){
			perror("error");
		}
	}
	else{
		if (chdir(args[1]) == -1){
			perror("error");
		}
	}
}

void command_initialiser(char **args){
	char* com[] = {"exit","pwd","cd","echo"};

	if (args[0] == NULL){
		printf("NO command is entered");
	}
	else if(strcmp(args[0],com[0])==0){
		exitfunc();
	}
	else if(strcmp(args[0],com[1])==0){
		PWD();
	}
	else if(strcmp(args[0],com[2])==0){
		cd(args);
	}
	else if(strcmp(args[0],com[3])==0){
		echo(args);
	}
	else{
		execute(args);
	}
}