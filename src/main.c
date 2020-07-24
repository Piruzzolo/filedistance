/* This file is part of filedistance
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
#include <string.h>
#include <signal.h>  // signal, SIGINT
#include <unistd.h>  // write, STDOUT_FILENO
#include <time.h>    // clock
#include <errno.h>   // errno
#include <limits.h>  // LONG_MAX, LONG_MIN
#include <stdbool.h> // bool

#include "../include/distance.h"
#include "../include/script.h"
#include "../include/apply.h"
#include "../include/search.h"


char* ABORT    = "\nSIGINT received. Stop.               \n";
char* NUMARGS  = "ERROR: Wrong number of arguments.      \n";
char* CANTSAVE = "ERROR: Can't save the output file.     \n";
char* CANTOPEN = "ERROR: can't open the file(s).         \n";
char* ONEARG   = "ERROR: expected at least one argument  \n";
char* DIDUMEAN = "Command not correct, did you mean '%s'?\n";
char* NODIGIT  = "ERROR: no digits were found.           \n";
char* NOTVALID = "ERROR: Command %s not valid.         \n\n";


void parse_int_or_fail(const char* str, long* v);
bool hint_didumean(const char* command);


void abort_handler()
{
    /* it's not advisable to call printf inside an interrupt handler */
    write(STDOUT_FILENO, ABORT, strlen(ABORT));
    exit(EXIT_FAILURE);
}


void hello()
{
    printf("--------------------------------------------------------------\n");
    printf("filedistance  Copyright (C) 2020  Marco Savelli               \n");
    printf("This program comes with ABSOLUTELY NO WARRANTY.               \n");
    printf("This is free software, and you are welcome to redistribute it \n");
    printf("under certain conditions. See 'LICENSE' for details           \n");
    printf("--------------------------------------------------------------\n");
}


void print_usage()
{
    printf("                                                             \n");
    printf("Usage: filedistance distance file1 file2 [output]            \n");
    printf("       filedistance apply inputfile filem outputfile         \n");
    printf("       filedistance search inputfile dir                     \n");
    printf("       filedistance searchall inputfile dir limit            \n");
    printf("       filedistance help                                     \n");
    printf("                                                             \n");
}


int main(int argc, char** argv)
{
    hello();

    /* handle SIGINT */
    signal(SIGINT, abort_handler);

    if (argc < 2)
    {
        printf("%s", ONEARG);
        print_usage();
    }

    if (strcmp(argv[1], "distance") == 0)
    {
        /* distance file1 file2 */
        if (argc == 4)
        {
            clock_t begin = clock();
                int result = distance_file(argv[2], argv[3]);
            clock_t end = clock();

            if (result < 0)
            {
                printf("%s", CANTOPEN);
                exit(EXIT_FAILURE);
            }

            printf("EDIT DISTANCE: %d\n", result);
            printf("TIME: %f\n", ((double)(end - begin)) / CLOCKS_PER_SEC);
        }

        /* distance file1 file2 output */
        else if (argc == 5)
        {
            int ret = script_file_distance(argv[2], argv[3],argv[4]);
            if (ret < 0)
            {
                printf("%s", CANTSAVE);
                exit(EXIT_FAILURE);
            }
            printf("DISTANCE: %d\n", ret);
            printf("Edit script saved successfully: %s\n", argv[4]);
        }
        else
        {
            printf("%s", NUMARGS);
            print_usage();
        }
    }

    else if (strcmp(argv[1], "apply") == 0)
    {
        /* apply inputfile filem outputfile */
        if (argc == 5)
        {
            if (apply_edit_script(argv[2], argv[3], argv[4]) != 0)
            {
                apply_print_err(errno);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            printf("%s", NUMARGS);
            print_usage(); exit(EXIT_FAILURE);
        }
    }

    else if (strcmp(argv[1], "search") == 0)
    {
        /* search inputfile dir */
        if (argc == 4)
        {
            search_min(argv[2], argv[3]);
        }
        else
        {
            printf("%s", NUMARGS);
            print_usage(); exit(EXIT_FAILURE);
        }
    }

    else if (strcmp(argv[1], "searchall") == 0)
    {
        /* searchall inputfile dir limit */
        if (argc == 5)
        {
            long limit = 0;
            parse_int_or_fail(argv[4], &limit);
            search_all(argv[2], argv[3], limit);
        }
        else
        {
            printf("%s", NUMARGS);
            print_usage(); exit(EXIT_FAILURE);
        }
    }

    /* help */
    else if (strcmp(argv[1], "help") == 0)
    {
        if (argc == 2)
        {
            print_usage();
        }
    }
    else
    {
        if (!hint_didumean(argv[1]))
        {
            printf(NOTVALID, argv[1]);
        }

        print_usage();
    }


    exit(EXIT_SUCCESS);
}


bool hint_didumean(const char* command)
{
    size_t lencmd = strlen(command);
    if (lencmd != 0)
    {
        char cmds[][9] = {"distance", "search", "apply", "searchall"};
        for (int i = 0; i < 4; i++)
        {
            int dist = distance_string(cmds[i], strlen(cmds[i]), command, lencmd);
            if (dist > 0 && dist <= 2)
            {
                printf(DIDUMEAN, cmds[i]);
                return true;
            }
        }
    }

    return false;
}


void parse_int_or_fail(const char* str, long* v)
{
    char* endptr;
    errno = 0;
    long val = strtol(str, &endptr, 10);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0))
    {
        exit(EXIT_FAILURE);
    }

    if (endptr == str)
    {
        fprintf(stderr, "%s", NODIGIT); exit(EXIT_FAILURE);
    }

    *v = val;
}