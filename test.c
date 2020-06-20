#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LEN 100 /* The maximum length command */

int main(void) {
  char *args[MAX_LEN / 2 + 1] = {NULL,}; /* command line arguments */
  char *cmds[10000] = {NULL,};
  int should_run = 1;          /* flag to determine when to exit program */
  int background = 0;
  int i;
  while (should_run) {
      printf("my_shell> ");
      fflush(stdout);
      char *input = (char*) malloc(MAX_LEN*sizeof(char));
      char *input2 = (char*) malloc(MAX_LEN*sizeof(char));
      input2 = NULL;
      fgets(input, MAX_LEN,stdin);
      input2 = strtok(input,";");
      i = 0;
      while (input2 != NULL)
      {
          // input2 = strtok(NULL," ");
          cmds[i++] = input2;
          printf("%s\n",input2);
          input2 = strtok(NULL,";");
      }
      cmds[i] = NULL;
      for(i = 0;cmds[i] !=  NULL;i++){
        printf("%s\n",cmds[i] = strtok(cmds[i]," "));
      }
      
      
  }
  return 0;
}


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