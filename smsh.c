#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX_LEN 100 /* The maximum length command */

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
      fgets(input, MAX_LEN,stdin);

      if(strcmp(input,"\n") == 0){
        goto start;
      }
      input = strtok(input,"\n"); //"\n" 제거

      //exit 종료
      if(strcmp(input,"exit") == 0){
        exit(0);
      }

	  if(strchr(input,'>') != NULL){
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

      // for (int i = 0; i < 10; i++)
      // {
      //     if (args[i] != NULL)           // 문자열 포인터 배열의 요소가 NULL이 아닐 때만
      //         printf("%s\n", args[i]);   // 문자열 포인터 배열에 인덱스로 접근하여 각 문자열 출력
      // }

      

      //printf("%s",input);
      /**
       * 표준입출력으로부터 문자열을 입력 받은 후:
       * (1) fork()를 통해 자식 프로세스를 생성
       * (2) 자식 프로세스가 execvp()를 호출하도록 할 것
       * (3) 만약 입력받은 문자에 &가 포함되어 있으면,
       *     부모 프로세스는 wait() 호출
       */
  }
  return 0;
}
