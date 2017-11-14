#include <stdlib.h>
#include <stdio.h>

#include <stdarg.h>
#include "dbg.h"

#include <ctype.h>
#include <string.h>

/*=============================================================================
 |
 |  Assignment:  
 |  Task number:  
 |
 |       Author:  Tomas Giedraitis
 |  Study group:  VU MIF INFO, 1st group
 |     Contacts:  tomasgiedraitis@gmail.com
 |        Class:  
 |         Date:  2017
 |
 |     Language:  GNU C (using gcc on Lenovo Y50-70, OS: Arch Linux x86_64)
 |     Version:   0.0
 |   To Compile:  gcc -Wall -xc -g -lm -std=c99 logfind.c -o logfind
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  
 |               
 |
 |  Constraints:  
 |                
 |
 |	    Input:    
 |               
 |
 |	    Output:   
 |                
 |                
 |
 | Version
 | updates:     Currently this is the intial version
 |
 +===========================================================================*/


#define MAX_LINE_SIZE 255

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/*printf(ANSI_COLOR_RED     "This text is RED!"     ANSI_COLOR_RESET "\n");*/
/*printf(ANSI_COLOR_GREEN   "This text is GREEN!"   ANSI_COLOR_RESET "\n");*/
/*printf(ANSI_COLOR_YELLOW  "This text is YELLOW!"  ANSI_COLOR_RESET "\n");*/
/*printf(ANSI_COLOR_BLUE    "This text is BLUE!"    ANSI_COLOR_RESET "\n");*/
/*printf(ANSI_COLOR_MAGENTA "This text is MAGENTA!" ANSI_COLOR_RESET "\n");*/
/*printf(ANSI_COLOR_CYAN    "This text is CYAN!"    ANSI_COLOR_RESET "\n");*/


// Description
//
//::params: 
//::params: 
void logfind(char *filename, FILE *input, char *word);


void removeSubstr (char *filename, int line, char *string, char *sub, int level) {
    char *match;
    int len_sub = strlen(sub);
    int len_string = strlen(string);


    match = strstr(string, sub);
    if (match && level == 0) {

            printf(ANSI_COLOR_CYAN    "%s"    ANSI_COLOR_RESET, filename);
            printf(ANSI_COLOR_GREEN ":%d:" ANSI_COLOR_RESET, line);
        }
    if (!match) {
        if (level == 0) {
            return;
        } else {
            printf(string);
            return;
        }
    }

    int len_match = strlen(match); 
    int match_start = len_string - len_match;

    for (int i = 0; i < match_start; i++) {
        printf("%c", string[i]);
    }

    printf(ANSI_COLOR_RED     "%s"     ANSI_COLOR_RESET, sub);

    int new_start = len_string - len_match + len_sub;

    // -1 for removing the newline char
    if (new_start != len_string -1 ) {
        char *new_string = string + new_start;
        removeSubstr(filename, line, new_string, sub, ++level);
    } else {
        printf("\n");
        return;
    }

}


int main(int argc, char *argv[]) {

   DIR *dp;
   struct dirent *ep;
   dp = opendir ("./");
   char *word = argv[1];

   if (dp != NULL)
   {
     while (ep = readdir (dp)) {
       /*puts (ep->d_name);*/

        char *filename = ep->d_name;
        FILE *input = fopen(filename, "r");

        char line[MAX_LINE_SIZE];

        int line_no = 1;

        while (fgets(line, sizeof(line), input)) {
            removeSubstr(filename, line_no++, line, word, 0);
        }

        fclose(input);
     }

     (void) closedir (dp);
   }
   else
     perror ("Couldn't open the directory");

   return 0;
}

/*int main(int argc, char *argv[]) {*/

    /*char *word = argv[1];*/
    /*FILE *input = fopen(argv[2], "r");*/
    /*char *filename = argv[2];*/

    /*if (!input) {*/
        /*printf("Such input file does not exist.\n");*/
        /*return 1;*/
    /*}*/

    /*char line[MAX_LINE_SIZE];*/

    /*int line_no = 1;*/

    /*while (fgets(line, sizeof(line), input)) {*/
        /*removeSubstr(filename, line_no++, line, word, 0);*/
    /*}*/

    /*fclose(input);*/

    /*return 0;*/

/*}*/


