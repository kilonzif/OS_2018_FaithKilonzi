/*
 * Author: Faith Kilonzi
 * File:   my-grep.c
 * This program implements the grep unix/linux command whereby 
 *it searches for a given word in a given file(s) supplied by the user
 * and prints out the lines in the file containing that particular word.
 */

#include<stdio.h>
#include<string.h>

void main(int argc , char *argv[])
{
               FILE *fp;
               char line[100];
               int counter;
             for (counter = 2; counter < argc; counter++)
		{

              fp = fopen(argv[counter],"r"); //Initialize the pointer to the file to read from
		        if(fp == 0) {
           		printf( "my-grep: cannot open file\n");
          		           
        	}
   
             // looping to read line by line and comparing each word in line with the search argument
             while(fscanf(fp , "%[^\n]\n" , line)!=EOF)

            {        //if the word is found print the entire line
                      if(strstr(line , argv[1]) !=NULL)
                     {
                             printf("%s\n" , line);
                      }
                     else
                    {
                            continue; //word not found
                    }
           }


            fclose(fp);
}
}
