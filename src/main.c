/* This file is part of FileDistance
*  Copyright (C) 2020  Marco Savelli
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

// glibc doesn't have strlcpy
#ifdef __GNU_LIBRARY__
    #include "../include/safe_str/strlcpy.h"
#endif

#include "../include/distance.h"
#include "../include/script.h"
#include "../include/search.h"


static char abortMsg[] = "\nCTRL-C received. Stop.\n";

void abortHandler()
{
    // it's not advisable to call printf inside an interrupt
    // handler cause it's not reentrant
    write(STDOUT_FILENO, abortMsg, strlen(abortMsg));
    exit(EXIT_FAILURE);
}


void print(edit* e)
{
    if (e->type == INS)
    {
        printf("Insert %c", e->arg2);
    }
    else if (e->type == DEL)
    {
        printf("Delete %c", e->arg1);
    }
    else
    {
        printf("Substitute %c for %c", e->arg2, e->arg1);
    }
    printf(" at %u\n", e->pos);
}


void hello()
{
    printf("-------------------------------------------------------------\n");
    printf("FileDistance  Copyright (C) 2020  Marco Savelli              \n");
    printf("This program comes with ABSOLUTELY NO WARRANTY.              \n");
    printf("This is free software, and you are welcome to redistribute it\n");
    printf("under certain conditions. See 'LICENSE' for details          \n");
    printf("-------------------------------------------------------------\n");
}


void print_usage()
{
    printf("Usage: filedistance distance file1 file2 [output]            \n");
    printf("       filedistance apply inputfile filem outputfile         \n");
    printf("       filedistance distance file1 file2 output              \n");
    printf("       filedistance search inputfile dir                     \n");
    printf("       filedistance searchall inputfile dir limit            \n");
    printf("       filedistance help                                     \n");
}


int main(int argc, char** argv)
{
    hello();

    // handle CTRL-C
    signal(SIGINT, abortHandler);

    if (argc < 2)
    {
        perror("Expected at least one argument\n");
        print_usage();
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "distance") == 0)
    {
        if (argc == 4)
        {
            time_t begin = time(NULL);
                int result = levenshtein_file_distance(argv[2], argv[3]);
            time_t end = time(NULL);

            if (result < 0)
            {
                printf("ERROR: Can't open the file(s)\n");
                exit(EXIT_FAILURE);
            }

            printf("EDIT DISTANCE: %d\n", result);
            printf("TIME: %ld\n", end - begin );
            exit(EXIT_SUCCESS);
        }
        else if (argc == 5)
        {
            int ret = levenshtein_file_distance_script(argv[2], argv[3], argv[4]);
            if (ret == 0)
            {
                exit(EXIT_SUCCESS);
            }
            else
            {
                printf("ERROR: Can't save the output file\n");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            perror("ERROR: Wrong number of arguments for 'distance' command\n");
            print_usage();
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "apply") == 0)
    {
        if (argc == 5)
        {
            //apply_edit_script(argv[2], argv[3], argv[4]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            perror("ERROR: Wrong number of arguments for 'apply' command\n");
            print_usage();
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "search") == 0)
    {
        if (argc == 4)
        {
            search_min(argv[2], argv[3]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            perror("ERROR: Wrong number of arguments for 'search' command\n");
            print_usage();
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "searchall") == 0)
    {
        if (argc == 5)
        {
            //search_all(argv[2], argv[3], argv[4]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            perror("ERROR: Wrong number of arguments for 'searchall' command\n");
            print_usage();
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "help") == 0)
    {
        if (argc == 2)
        {
            print_usage();
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        print_usage();
    }

    edit* script;

    unsigned int distance1 = 0;
    unsigned int distance2 = 0;

    const char str1[] = "aggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggagg";
    const char str2[] = "ddsasssssaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggaggagg";

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    edit* ss = NULL;
    distance1 = levenshtein_create_script(ss, str1, len1, str2, len2);

    //distance2 = levdistOpt(str1, strlen(str1) + 1, str2, strlen(str2) + 1);

    printf("Distance is : %d\n", distance1);
    //printf("Distance is %d:\n", distance2);

    //for (int i = 0; i < distance1; i++)
    //{
    //    print(&script[i]);
    //}

    //free(script);

    exit(0);
}