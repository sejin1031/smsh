#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX_LEN 100 /* The maximum length command */


int join(char *com1[], char *com2[]){
  int p[2], status;

  switch (fork())
  {
  case -1:
    perror("1st fork error");
    break;
  
  case 0:
    break;
    
  default:
  wait(&status);
  return status;


  }

  if (pipe(p) == -1)
    perror("pipe create error");
  switch (fork())
  {
  case -1:
    perror("fork 2 error");
  case 0:
  dup2(p[1],1);
  close(p[0]);
  close(p[1]);
  execvp(com1[0],com1);
  perror("1st execvp error");
  
  default:
  dup2(p[0],0);
  close(p[0]);
  close(p[1]);
  execvp(com2[0],com2);
  perror("1st execvp error");
  }
}

void splitcmd(char *input, char *args[MAX_LEN / 2 + 1],int background){
	background = 0;
	  memset(args,'\0',sizeof(char)*(MAX_LEN / 2 + 1));
      int i = 0;
      char *ptr = strtok(input, " ");

      while (ptr != NULL)
      {
          
          if(strcmp(ptr,"&") == 0){
            background = 1;
            args[i] = NULL;
            break;
          } else{
            args[i] = ptr;
            i++;
            ptr = strtok(NULL, " ");
          }
      }
}

int main(void) {
  char *args[MAX_LEN / 2 + 1] = {NULL,}; /* command line arguments */
  char *args2[MAX_LEN / 2 + 1] = {NULL,};
  int should_run = 1;          /* flag to determine when to exit program */
  int background = 0;
  int history = 0;
  char historys[1000][1000] = {'\0',};
  char pwd[MAX_LEN];

  char *text1 = NULL;
  char *text2 = NULL;
  
  int fdr;

  while (should_run) {
    start:
	  getcwd(pwd,MAX_LEN);
      printf("%s$",pwd);
      fflush(stdout);
      char *input = (char*) malloc(MAX_LEN*sizeof(char));
	  char cmd[1000];
      fgets(input, MAX_LEN,stdin);

      if(strcmp(input,"\n") == 0){
        goto start;
      }
      input = strtok(input,"\n"); //"\n" 제거
	  strcpy(historys[history++],input);
      //exit 종료
      if(strcmp(input,"exit") == 0){
        exit(0);
      }

	  if(strcmp(input,"history") == 0){
		  int num;
		  for(num=0; *historys[num] != '\0' ;num++){
			  printf("%4d %s\n",num+1,historys[num]);
		  }
		  goto start;
	  }
	  else if(strstr(input,">>") != NULL){
			printf(">> 실행\n");
		  text1 = strtok(input,">");
		  text2 = strtok(NULL,">");
		  splitcmd(text1, args,background);
		  splitcmd(text2, args2,background);

		  switch (fork()){
		 	case -1 : perror ("fork"); 
				break;
			case 0 :
				fdr = open(args2[0], O_WRONLY | O_CREAT |O_APPEND, 0644);
				if(fdr==-1) {
					perror("파일 새로생성 오류");
					exit(1);
				}
				if( dup2(fdr, 1) == -1){
					perror("fdr dup error");
				}
				close(fdr);
				execvp(args[0], args);
				printf("command not found \n");     
				exit(0);        
				break;
			default : wait(NULL);
			  
		  }
	  }else if(strstr(input,">!") != NULL){
		  text1 = strtok(input,">");
		  text2 = strtok(NULL,">!");
		  printf("text1 = %s, text2 = %s \n",text1, text2);
		  splitcmd(text1, args,background);
		  splitcmd(text2, args2,background);

		  switch (fork()){
		 	case -1 : perror ("fork"); 
				break;
			case 0 :
				fdr = open(args2[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if(fdr==-1) {
					perror("파일 새로생성 오류");
					exit(1);
				}
				if( dup2(fdr, 1) == -1){
					perror("fdr dup error");
				}
				close(fdr);
				execvp(args[0], args);
				printf("command not found \n");     
				exit(0);        
				break;
			default : wait(NULL);
			  
		  }
	  } 
	  else if(strchr(input,'>') != NULL){
		  text1 = strtok(input,">");
		  text2 = strtok(NULL,">");
		  splitcmd(text1, args,background);
		  splitcmd(text2, args2,background);

		  switch (fork()){
		 	case -1 : perror ("fork"); 
				break;
			case 0 :
				fdr = open(args2[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if(fdr==-1) {
					perror("파일 새로생성 오류");
					exit(1);
				}
				if( dup2(fdr, 1) == -1){
					perror("fdr dup error");
				}
				close(fdr);
				execvp(args[0], args);
				printf("command not found \n");     
				exit(0);        
				break;
			default : wait(NULL);
			  
		  }
	  } else if(strchr(input,'<') != NULL){
		  text1 = strtok(input,"<");
		  text2 = strtok(NULL,"<");
		  splitcmd(text1, args,background);
		  splitcmd(text2, args2,background);

		  switch (fork()){
		 	case -1 : perror ("fork"); 
				break;
			case 0 :
				fdr = open(args2[0], O_RDONLY);
				close(0);
				if(fdr==-1) {
					perror("파일 새로생성 오류");
					exit(1);
				}
				if( dup2(fdr, 0) == -1){
					perror("fdr dup error");
				}
				execvp(args[0], args);
				printf("command not found \n");
				exit(0);
				break;
			default : wait(NULL);
		  }
	  }
	  else if(strchr(input,'|') != NULL){
		  text1 = strtok(input,"|");
		  text2 = strtok(NULL,"|");
		  splitcmd(text1, args,background);
		  splitcmd(text2, args2,background);

		  join(args,args2);

		  
	  }
      else{
		  splitcmd(input,args,background);
      

	  //change directory
		if(strcmp(args[0],"cd") == 0){
			chdir(args[1]);
			goto start;
		}

		int pid;
		int status;
		if(strchr)
		pid = fork();
		if(pid < 0){
			fprintf(stderr, "Fork Failed");
			return 1;
		} else if( pid == 0){
			execvp(args[0],args);
		} else{
			if(!background){
			waitpid(pid,&status,0);
			}
		}
	  }

      free(input);

  }
  return 0;
}
