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

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ftw.h>     // ftw
#include <limits.h>  // INT_MAX
#include <stdbool.h>
#include <string.h>  // strcmp

#include "../include/search.h"
#include "../include/list.h"
#include "../include/distance.h"
#include "../include/name_distance.h"
#include "../include/list_namedistance.h"

#define MAX_OPEN_FD 8 // max dirs open at the same time

char* inputFile;

node* list = NULL;

long lim = INT_MAX;

bool compare_fun(void* pVoid, op_t op, int value)
{
    int dist = ((name_distance*) pVoid)->distance;

    switch (op)
    {
        case EQUAL_TO:
        {
            return dist == value;
        }
        case LESS_THAN:
        {
            return dist < value;
        }
        case GTR_THAN:
        {
            return dist > value;
        }
        case EQ_LESS_THAN:
        {
            return dist <= value;
        }
        case EQ_GTR_THAN:
        {
            return dist >= value;
        }

        default:
            return false;
    }
}



int add_file(const char *fname, const struct stat *st, int type)
{
    /* must be a regular file */
    if (type == FTW_D)
        return 0;

    /* file > lim has always distance > lim */
    if (st->st_size > lim)
    {
        /* dont't process */
        return 0;
    }

    int distance = levenshtein_file_distance(fname, inputFile);
    if (distance < 0)
    {
        return -1;
    }

    name_distance* fd = (name_distance*) malloc(sizeof(name_distance));
    if (!fd)
    {
        return -1;
    }

    fd->distance = distance;

    char resolvedPath[PATH_MAX + 1];

    char* ptr = realpath(fname, resolvedPath);

    /* copy & detect truncation */
    if (strlcpy(fd->filename, ptr, sizeof(fd->filename)) >= sizeof(fd->filename))
    {
        // error
        return -1;
    }

    if (list == NULL)
    {
        /* create list if first element */
        list = list_create(fd, NULL);
    }
    else
    {
        /* append data to list */
        list_append(list, fd);
    }

    return 0;
}


int cmpfunc(const void* a, const void* b)
{
    name_distance* nd1 = (name_distance*) a;
    name_distance* nd2 = (name_distance*) b;

    if (nd1->distance < nd2->distance)
    {
        return -1;
    }
    else if (nd1->distance > nd2->distance)
    {
        return 1;
    }
    else
    {
        return strcmp(nd1->filename, nd2->filename);
    }
}


int search_min(const char* f, const char* dir)
{
    if (f == NULL || dir == NULL)
    {
        //perror ...
        return -1;
    }

    struct stat st;
    stat(f, &st);

    lim = st.st_size;

    inputFile = (char*) f;

    /* dir traversal, 8 open dirs max */
    int res = ftw(dir, add_file, MAX_OPEN_FD);
    if (res != 0)
    {
        // error
    }

    /* find min of list */
    int min = list_namedistance_min(list);

    /* filter list in place, keep elems w/ distance = min */
    node* filterd = list_filter(list, EQUAL_TO, min, (comparison_f) compare_fun);

    /* print filenames */
    list_traverse(filterd, (callback_t) list_namedistance_print_name);

    /* frees up the list */
    list_free(filterd);

    return 0;
}


void print_fd(name_distance nd)
{
    printf("%d %s\n", nd.distance, nd.filename);
}


int search_all(const char* f, const char* dir, long limit)
{
    if (f == NULL || dir == NULL)
    {
        //perror ...
        return -1;
    }

    lim = limit;

    inputFile = (char*) f;

    /* dir traversal, 8 open dirs max */
    int res = ftw(dir, add_file, MAX_OPEN_FD);
    if (res != 0)
    {
        // error
    }

    /* filter list in place, keep elems w/ distance <= limit */
    node* filtered = list_filter(list, EQ_LESS_THAN, limit, (comparison_f) compare_fun);

    int len = list_count(filtered);

    /* no files found, return */
    if (len <= 0)
        return 0;

    /* save list to array */
    name_distance* arr = NULL;
    list_namedistance_save_to_array(filtered, &arr);

    /* free list */
    list_free(filtered);

    /* order by distance asc, filename asc */
    qsort(arr, len, sizeof(name_distance), cmpfunc);

    /* print all */
    for (int i = 0; i < len; i++)
    {
        print_fd(arr[i]);
    }

    return 0;
}