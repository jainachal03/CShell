#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#define MAXSIZE 1000
#define DELIM " \t\r\n\a"


char* read_line(){
	int bufferSize = MAXSIZE;
	int position = 0;
	char* buffer = malloc(sizeof(char) * bufferSize);
	int c;
	if(!buffer){
		fprintf(stderr, "\nallocation error\n");
		exit(EXIT_FAILURE);
	}
	while(1){
		c = getchar();
		if(c == EOF ||  c == '\n'){
			buffer[position] = '\0';
			return buffer;
		}else{
			buffer[position] = c;
		}
		++position;
		if(position>=bufferSize){
			bufferSize += bufferSize;
			buffer = realloc(buffer, bufferSize);
			if(!buffer){
				fprintf(stderr, "\n allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}
char** split_line(char* line){
	
	int buffsize =	MAXSIZE, position = 0;
	char** tokens = malloc(buffsize * sizeof(char*));
	char* token;
	if(!tokens){
		fprintf(stderr, "\nallocation error \n");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, DELIM); // usng white space as the delimiter.
	while(token != NULL){
		tokens[position] = token;
		position++;
		if(position>=buffsize){
			buffsize += buffsize;
			tokens = realloc(tokens, buffsize * sizeof(char*));
			if(!tokens){
				fprintf(stderr, "\nallocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}
int execute(char** args){
	pid_t pid, wpid;
	int status;

	pid = fork();
	if(pid == 0){
		if(execvp(args[0], args) != -1){
			perror("lsh");
		}
		exit(EXIT_FAILURE);
	}else if(pid<0){
		// error forkin
		perror("lsh");
	}else{
		do{
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

void loop(){
	char *line;
	char **args;
	int status;
	do{
		printf("\n>");
		line = read_line();
		args = split_line(line);
		status = execute(args);
		free(line);
		free(args);
	}while(status);
}

int main(){
	loop();
	
	return EXIT_SUCCESS;
}

