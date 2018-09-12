/*
 * Author: Faith Kilonzi
 * File:   my-cat.c
 * This program implements the cat unix/linux command whereby 
 *it reads a given file supplied by the user and prints out the contents
 * of the file using C (high-level language)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char **argv)
{
    int i;  // counter to loop through argv file argument 
    FILE *fil;  //the file to be opened
    int ch;

    for(i = 1; i < argc; i++) {
        fil = fopen(argv[i], "r"); // opens a file and returns its handle 
        if(fil == 0) {
           printf( argv[1],"cannot open file\n");
           exit(1);
          return 1;
        }

        while((ch = getc(fil)) != EOF) {
             putchar(ch);    //reading and printing out file contents
         } 

        fclose(fil); //close the file
    }

    return 0;
}
