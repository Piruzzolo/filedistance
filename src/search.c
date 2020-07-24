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

#include <dirent.h>
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


/* max dirs open at the same time */
#define MAX_OPEN_FD 8

char* inputFile = NULL;
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


int add_file(const char* fname, const struct stat* st, int type)
{
    /* must be a regular file */
    if (type == FTW_D)
        return 0;

    /* file > lim has always distance > lim */
    if (st->st_size > lim)
    {
        /* don't process */
        return 0;
    }

    /* get distance to inputFile */
    int distance = distance_file(fname, inputFile);
    if (distance < 0)
    {
        return -1;
    }

    /* create node */
    name_distance* fd = (name_distance*) malloc(sizeof(name_distance));
    if (!fd)
    {
        return -1;
    }
    memset(fd, 0, sizeof(name_distance));

    /* resolve path to absolute */
    char resolvedPath[PATH_MAX + 1];
    char* ptr = realpath(fname, resolvedPath);
    if (!ptr)
    {
        return -1;
    }

    /* set node data: distance and filename */
    /* copy & detect truncation */

    fd->distance = distance;
    if (strlcpy(fd->filename, ptr, sizeof(fd->filename)) >= sizeof(fd->filename))
    {
        /* error */
        return -1;
    }

    /* add to list */

    if (list == NULL)
    {
        /* create list if first node */
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
        return -1;
    }

    /* get size of f */
    struct stat st;
    stat(f, &st);
    int sizef = st.st_size;

    /* set up parameters needed inside add_file
       can't pass directly bc of ftw callback signature constraint */
    lim = sizef;
    inputFile = (char*) f;

    /* dir traversal, MAX_OPEN_FD open dirs max */
    int res = ftw(dir, add_file, MAX_OPEN_FD);
    if (res != 0)
    {
        return -1;
    }

    /* find min of list */
    int min = list_namedistance_min(list);

    /* filter list in place, keep elems w/ distance == min */
    node* filterd = list_filter(list, (comparison_f) compare_fun, EQUAL_TO, min);

    /* print filenames */
    list_traverse(filterd, (callback_t) list_namedistance_print_name);

    /* frees up the list */
    list_free(filterd);

    return 0;
}


int search_all(const char* f, const char* dir, long limit)
{
    if (!f || !dir)
    {
        return -1;
    }

    /* set up parameters needed inside add_file */
    /* can't pass directly bc of ftw callback signature constraint */

    lim = limit;
    inputFile = (char*) f;

    /* dir traversal, MAX_OPEN_FD open dirs max */
    int res = ftw(dir, add_file, MAX_OPEN_FD);
    if (res != 0)
    {
        return -1;
    }

    /* filter list in place, keep elems w/ distance <= limit */
    node* filtered = list_filter(list, (comparison_f) compare_fun, EQ_LESS_THAN, limit);

    /* get number of nodes in list */
    int len = list_count(filtered);

    /* no files found, return */
    if (len <= 0)
        return 0;

    /* save list to array */
    name_distance* arr = NULL;
    if (list_namedistance_save_to_array(filtered, &arr) == -1)
    {
        /* free list */
        list_free(filtered);
        return -1;
    }

    /* free list */
    list_free(filtered);

    /* order by distance asc, filename asc */
    qsort(arr, len, sizeof(name_distance), cmpfunc);

    /* print all */
    for (int i = 0; i < len; i++)
    {
        namedistance_print(&arr[i]);
    }

    return 0;
}