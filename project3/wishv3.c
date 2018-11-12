
/******************************************************************************************
  @file         wishv3.c
  @author       Faith Kilonzi
  @date         12th Nov 2018
  @brief       Implementation of unix shell in c- 
  featurendOfString: - parallel programs, redirection, errors
***********************************************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
//#include <sys/typendOfString.h>
#include <sys/stat.h>
#include <sys/wait.h>


#define MAXARGS 10

/**
Initializing structs for the commands

**/
struct cmd {
  int type;      //user command type (eg > &)   
};

struct executeCommands {
  int type;             
  char *argv[MAXARGS];  
};

struct redirectionCommand {
  int type;       
  struct cmd* cmd;   
  char *file;        
  int flags;        
  int fd;           
};

struct pipecmd {
  int type;          
  struct cmd* left; 
  struct cmd* right; 
};

struct parallelCommands{
  int type;
  struct cmd* left;
  struct cmd* right;
};

int fork1(void);  
struct cmd *parseCommands(char*);


void error(){
  char error_mendOfStringsage[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_mendOfStringsage, strlen(error_mendOfStringsage)); 
}


void executeCommand(struct cmd* cmd){
  int p[2], r;
  struct executeCommands *ecmd;
  struct pipecmd *pcmd;
  struct redirectionCommand *rcmd;
  struct parallelCommands *paracmd;

  if(cmd == 0)
    _exit(0);
  
  switch(cmd->type){
  default:
    error();
    _exit(-1);
  case ' ':
    ecmd = (struct executeCommands*) cmd;    
    char path[100] = "/bin/";     
    if(ecmd->argv[0] == 0){
      _exit(0);
    }

    if(execv(strcat(path, ecmd->argv[0]), ecmd->argv) == -1){
      error();
    }
    
    break;
  case '<':
  case '>':
    rcmd = (struct redirectionCommand*)cmd;
    close(rcmd->fd);                                                           
    if (open(rcmd->file, rcmd->flags) < 0) {  
    error();                
    }                                   
    executeCommand(rcmd->cmd);
    break;

  case '|':
    pcmd = (struct pipecmd*)cmd;

    break;
  

  case '&':
    paracmd = (struct parallelCommands*)cmd;

    if(fork1()== 0){
      executeCommand(paracmd->left);
    }else if(fork1()==0){
      executeCommand(paracmd->right);
    }else{
      wait(&r);
      wait(&r);
    }

    break;
 }   

}

int getUserCommand(char *buf, int nbuf)
{


  if (isatty(fileno(stdin)))       
    fprintf(stdout, "wish > ");                      
  memset(buf, 0, nbuf);

  if(fgets(buf, nbuf, stdin) == 0){     
    return -1; // EOF
  }                                        
  return 0;
}



/***main method Driver ***/

int main(int argc, char *argv[]) {
      FILE *stream;

  ///interactive mode
    if(argc==1)
    {
          static char buf[100];
          int fd, r;


          while(getUserCommand(buf, sizeof(buf)) >= 0)
          { 
            if(buf[strlen(buf)-2] == '&'){
              error();
              continue;
              }
            if(buf[strlen(buf)-2] == '>'){
              error();
              continue;
              }
              
            if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
             
              buf[strlen(buf)-1] = 0;  
              if(chdir(buf+3) < 0){    
              error();                      
              fprintf(stderr, "cannot cd %s\n", buf+3);
              continue;
              }
          
            }

            if(fork1() == 0){
              executeCommand(parseCommands(buf));
            }
            wait(&r);       
          }

       }

       //batch mode
       else if(argc==2)
       {

          printf("%s\n","You are on batch mode " );
       
            stream = fopen(argv[1], "r");
            if (stream == NULL) {
                    error();
                    exit(1);
               }

      }
      else
      {
        printf("%s\n","More than 2 arguments are Invalid " );
        exit(1);
      }
     return 0;
}

/*end of main method**/


int fork1(void)
{
  int pid;
  pid = fork();      
  if(pid == -1)
    perror("fork");
  return pid;
}

//Struc for command Execution
struct cmd* executeCommands(void) {
  struct executeCommands *cmd;
  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = ' ';
  return (struct cmd*)cmd;
}


//Struc for redirection functionality

struct cmd* redirectionCommand (struct cmd *subcmd, char *file, int type)
{
  struct redirectionCommand *cmd;
  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = type;
  cmd->cmd = subcmd;
  cmd->file = file;
  cmd->flags = (type == '<') ?  O_RDONLY : O_WRONLY|O_CREAT|O_TRUNC;
  cmd->fd = (type == '<') ? 0 : 1;
  return (struct cmd*)cmd;
}


//Struc for pipe functionality *Not required
struct cmd* pipecmd(struct cmd *left, struct cmd *right)
{
  struct pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = '|';
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}


//Struc for parallel commands functionality
struct cmd* parallelCommands(struct cmd *left, struct cmd *right){
  struct parallelCommands *cmd;
  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = '&';
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}


// Parsing tokenization input

char whitendOfStringpace[] = " \t\r\n\v";
char symbols[] = "<|&>";            

int tokenizeInput(char **parsedString, char *endOfString, char **q, char **eq) 
{                                                           
  char *s;   
  int returnValue;
  
  s = *parsedString;    
  while(s < endOfString && strchr(whitendOfStringpace, *s)){      
    s++;                                       

}     
  
  if(q){
    *q = s;              
  }

  returnValue = *s;  
  switch(*s){
  case 0:           
    break;

  case '|':
  case '&':       
    s++;
    break;

  case '<':
    s++;
    break;

  case '>':
    s++;
    break;

  default:                                    
    returnValue = 'a';
    while(s < endOfString && !strchr(whitendOfStringpace, *s) && !strchr(symbols, *s))
      s++;
    break;
  } 

  if(eq){
    *eq = s;       
  }               

  while(s < endOfString && strchr(whitendOfStringpace, *s)){
    s++;
  }       

  *parsedString = s;       
  return returnValue;
}


int peek(char **parsedString, char *endOfString, char *toks)  
{
  char *s;  
  s = *parsedString;
  while(s < endOfString && strchr(whitendOfStringpace, *s))
    s++;
  *parsedString = s;
  return *s && strchr(toks, *s);  
}

struct cmd *parseline(char**, char*);
struct cmd *parsepipe(char**, char*);
struct cmd *parseexec(char**, char*);
struct cmd *parseparallel(char**, char*);              



/** make a copy of the characters in the input buffer, starting from sstart of string to end.
// null-terminate the copy to make it a string.
***/
char* mkcopy(char *s, char *endOfString)
{
  int n = endOfString - s;
  char *c = malloc(n+1);
  assert(c);
  strncpy(c, s, n);
  c[n] = 0;
  return c;
}


/***
  Parse the given commands

**/
struct cmd* parseCommands(char *s)
{
  char *endOfString;
  struct cmd *cmd;

  endOfString = s + strlen(s);
  cmd = parseline(&s, endOfString);
  peek(&s, endOfString, "");
  if(s != endOfString){
    error();
    exit(-1);
  }
  return cmd;
}



struct cmd* parseline(char **parsedString, char *endOfString)   
{
  struct cmd *cmd;
  if(peek(parsedString, endOfString, "|")){
  cmd = parsepipe(parsedString, endOfString) ;
  }else{
    cmd = parseparallel(parsedString, endOfString);
  }
  return cmd;
}


struct cmd* parsepipe(char **parsedString, char *endOfString){
  struct cmd *cmd;

  cmd = parseexec(parsedString, endOfString);
  if(peek(parsedString, endOfString, "|")){
    tokenizeInput(parsedString, endOfString, 0, 0);
    cmd = pipecmd(cmd, parsepipe(parsedString, endOfString));
  }
  return cmd;
}


struct cmd* parseparallel(char **parsedString, char *endOfString){       
  struct cmd *cmd;
  cmd = parseexec(parsedString, endOfString);
  if(peek(parsedString, endOfString, "&")){
    tokenizeInput(parsedString, endOfString, 0, 0);
    cmd = parallelCommands(cmd, parseparallel(parsedString, endOfString));
  }
  return cmd;
}


struct cmd* parseredirs(struct cmd *cmd, char **parsedString, char *endOfString)
{
  int tok;
  char *q, *eq;

  while(peek(parsedString, endOfString, "<>")){
    tok = tokenizeInput(parsedString, endOfString, 0, 0);
    if(tokenizeInput(parsedString, endOfString, &q, &eq) != 'a') {
      fprintf(stderr, "missing file for redirection\n");
      exit(-1);
    }
    switch(tok){
    case '<':
      cmd = redirectionCommand(cmd, mkcopy(q, eq), '<');
      break;
    case '>':
      cmd = redirectionCommand(cmd, mkcopy(q, eq), '>');
      break;
    }
  }
  return cmd;
}

struct cmd* parseexec(char **parsedString, char *endOfString)
{
  char *q, *eq;
  int tok, argc;
  struct executeCommands *cmd;
  struct cmd *returnValue;
  
  returnValue = executeCommands();
  cmd = (struct executeCommands*)returnValue;

  argc = 0;
  returnValue = parseredirs(returnValue, parsedString, endOfString);
  while(!peek(parsedString, endOfString, "&")){   
    if((tok=tokenizeInput(parsedString, endOfString, &q, &eq)) == 0)
      break;
    if(tok != 'a') {
      error();
      exit(-1);
    }
    cmd->argv[argc] = mkcopy(q, eq);
    argc++;
    if(argc >= MAXARGS) {
      error();
            exit(-1);
    }
    returnValue = parseredirs(returnValue, parsedString, endOfString);
  }
  cmd->argv[argc] = 0;
  return returnValue;
}