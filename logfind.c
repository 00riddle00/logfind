/*=============================================================================
  |
  |     Language:  GNU C 
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
  |      Input:
  |
  |
  |      Output:
  |
  +===========================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <fts.h>
#include <string.h>
#include <dirent.h>

#include "dbg.h"


#define MAX_LINE_SIZE 255

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


// Description
//
//::params:
//::params:
static int ptree(char *const argv[], char * word);


/// Description
//
//::params:
//::params:
void removeSubstr (char *filename, int line, char *string, char *sub, int level);


int main(int argc, char *const argv[])
{
    char *word = argv[1];

    int rc;

    if ((rc = ptree(argv + 2, word)) != 0)
        rc = 1;
    return rc;
}


static int ptree(char *const argv[], char * word)
{

    int rc;
    FTS *ftsp;
    FTSENT *p, *chp;
    int fts_options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;
    int rval = 0;

    if ((ftsp = fts_open(argv, fts_options, NULL)) == NULL) {
        warn("fts_open");
        return -1;
    }
    /* Initialize ftsp with as many argv[] parts as possible. */
    chp = fts_children(ftsp, 0);
    if (chp == NULL) {
        return 0;               /* no files to traverse */
    }
    while ((p = fts_read(ftsp)) != NULL) {

        char *filename = p->fts_path;
        FILE *input = fopen(filename, "r");

        char line[MAX_LINE_SIZE];

        int line_no = 1;

        while (fgets(line, sizeof(line), input)) {
            removeSubstr(filename, line_no++, line, word, 0);
        }

        fclose(input);
    }

    fts_close(ftsp);
    return 0;
}


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
