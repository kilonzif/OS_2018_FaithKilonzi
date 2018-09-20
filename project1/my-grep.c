/*
 * Author: Faith Kilonzi
 * File:   my-grep.c
 * This program implements the grep unix/linux command whereby 
 *it searches for a given word in a given file(s) supplied by the user
 * and prints out the lines in the file containing that particular word.
 */

#include<stdio.h>
#include<string.h>
#include <stdlib.h>

int main(int argc , char *argv[])
{
         FILE *fp;
         char line[100];
         int counter;
         for (counter = 2; counter < argc; counter++){
            fp = fopen(argv[counter],"r"); //pointer to open and the file
		if(fp == 0) {
                 	printf( "my-grep: cannot open file\n");  
                        exit(1);    
        	}

	
        // looping to read line by line and comparing each word with search argument
             while(fscanf(fp , "%[^\n]\n" , line)!=EOF)

            {        //if the word is found print the entire line
                      if(strstr(line , argv[1]) !=NULL)
                     {
                             printf("%s\n" , line);
                      }
                    
           }
	  fclose(fp);
} 
if(argv[2]==NULL && argv[1]!=NULL){
	char str[20]; 
   scanf("%[^\n]%*c", str); 
   if(strstr(str,argv[1]) !=NULL){
   printf("%s\n", str); }
}

    return (0);
    exit(0);  


}
