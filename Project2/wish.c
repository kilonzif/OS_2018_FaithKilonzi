/*
 * Author: Faith Kilonzi
 * File: wish.c
 * This program implements the unix/linux shell
 * Mode: interactive mode and batch mode.
 */


	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>
	#include <errno.h>
	#include <sys/wait.h>



	int main(int argc, char *argv[]) {
	        char *line;
	        size_t bufsize = 32;
	    char *binPath = "/bin/";
	    char *userPath = "/usr/bin/";
	    char *params[20];

	        line = (char *)malloc(bufsize * sizeof(char));

	        if( line == NULL) {
	          perror("Unable to allocate line");
	                exit(1);
	        }
	        //interactive mode
	        if( argc == 1) {
	                /*char user[MAXCHAR];*/
	                while(1) {
	                        printf("wish> ");
	                        getline(&line, &bufsize, stdin);
	                        char length=strlen(line);
	                        if(line[length-1] == '\n'){
	                            line[length-1] = '\0';
	                        }
							int i =0;
                           // splitting by space
                          char *token = strtok(line, " ");
                          char *array[10] ={NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,NULL};
                          while(token != NULL)
                          {
                              params[i] = token;
                              //printf("%s",token);
                              i =i +1;
                              token = strtok(NULL, " ");

                          }
                          for(int j=1; j<i;j++){
                              array[j]=params[j];
                          }
                          char  *path1 = malloc(strlen(binPath)+ strlen(params[0]) +1 ); //al$
                          strcat(path1, binPath); //concantinate bin to empty string
                          //printf("%s\n", path1);
                         strcat(path1,params[0]); //add actual path to the new one
                         char  *path2 = malloc(strlen(userPath)+ strlen(params[0]) +1 );
                         strcat(path2, userPath);
                         strcat(path2, params[0]);

                         if(strcmp(params[0],"exit")==0){
                             exit(0);
                         }
                         else if(strcmp(params[0],"cd")==0){
	                         if (chdir(params[1])==0)
	                            {
	                                printf("%s\n","you entered the folder" );
	                             }else{
	                                printf("%s\n","folder not available" );
	                              }
	                     }
	                     else{
	            
			                pid_t pid = fork();
			                if(pid==0){
			                    if(access(path1,X_OK)==0){
			                        array[0]=path1;
			                        execv(path1,array);
			                    }
			                    else if(access(path2,X_OK)==0){
			                        array[0]=path2;                        
			                        execv(path2,array);
			                    }else{
			                        printf("%s\n","access denied for both paths" );
			                    }
			                }
			                else{
			                   wait(NULL);
			                }
	                  	 }

	                 }


	             }       //batch mode
	            else if(argc==2){

	            FILE *fpointer;
	            fpointer = fopen(argv[1],"r");
	            if(fpointer== NULL){
	                           printf( "cannot open BATCH file");
	                           exit(1);
	                   }

	                   while (getline(&line, &bufsize, fpointer)!=-1){
	                    char length=strlen(line);
	                        if(line[length-1] == '\n'){
	                            line[length-1] = '\0';
	                        }
							int i =0;
                           // splitting by space
                          char *token = strtok(line, " ");
                          char *array[10] ={NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,NULL};
                          while(token != NULL)
                          {
                              params[i] = token;
                              //printf("%s",token);
                              i =i +1;
                              token = strtok(NULL, " ");

                          }
                          for(int j=1; j<i;j++){
                              array[j]=params[j];
                          }
                          char  *path1 = malloc(strlen(binPath)+ strlen(params[0]) +1 ); //al$
                          strcat(path1, binPath); //concantinate bin to empty string
                         strcat(path1,params[0]); //add actual path to the new one
                         char  *path2 = malloc(strlen(userPath)+ strlen(params[0]) +1 );
                         strcat(path2, userPath);
                         strcat(path2, params[0]);

                         if(strcmp(params[0],"exit")==0){
                             exit(0);
                         }
                         else if(strcmp(params[0],"cd")==0){
	                         if (chdir(params[1])==0)
	                            {
	                                printf("%s\n","you entered the folder" );
	                             }else{
	                                printf("%s\n","folder not available" );
	                              }
	                     }
	                     else{
	            
			                pid_t pid = fork();
			                if(pid==0){
			                    if(access(path1,X_OK)==0){
			                        array[0]=path1;
			                        execv(path1,array);
			                    }
			                    else if(access(path2,X_OK)==0){
			                        array[0]=path2;                        
			                        execv(path2,array);
			                    }else{
			                        printf("%s\n","access denied for both paths" );
			                    }
			                }
			                else{
			                   wait(NULL);
			                }
	                  	 }
	           		

	        	}
	        	fclose(fpointer);
	        }
	         return 0;
	     }








