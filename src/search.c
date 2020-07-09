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

#include "../include/safestr.h"

char* inputFile;
node* list = NULL;
name_distance* fdList = NULL;

#define MAX_OPEN_FD 8 // max handles open at the same time

_Bool compare_fun(void* pVoid, long op, int value)
{
    switch (op)
    {
        case EQUAL_TO:
        {
            return ((name_distance*) pVoid)->distance == value;
        }
        case LESS_THAN:
        {
            return ((name_distance*) pVoid)->distance < value;
        }
        case GTR_THAN:
        {
            return ((name_distance*) pVoid)->distance > value;
        }
        case EQ_LESS_THAN:
        {
            return ((name_distance*) pVoid)->distance <= value;
        }
        case EQ_GTR_THAN:
        {
            return ((name_distance*) pVoid)->distance >= value;
        }

        default:
            return false;
    }
}


int min_list_file_distance(node* head)
{
    int min = INT_MAX;

    while (head != NULL)
    {
        if (min > ((name_distance*) head->data)->distance)
            min = ((name_distance*) head->data)->distance;

        head = head->next;
    }

    return min;
}


int add_file(const char *fname, const struct stat *st, int type)
{
    if (type == FTW_D)
        return 0;

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
    char* ptr;
    ptr = realpath(fname, resolvedPath);

    // copy & detect truncation
    if (strlcpy(fd->filename, ptr, sizeof(fd->filename)) >= sizeof(fd->filename))
    {
        // error
        return -1;
    }

    if (list == NULL)
    {
        list = create(fd, NULL);
    }
    else
    {
        append(list, fd);
    }

    return 0;
}

void print_node_name(node* node)
{
    name_distance* fd = ((name_distance*) node->data);

    printf("%s\n", fd->filename);
}


int cmpfunc(const void* a, const void* b)
{
    name_distance* nd1 = ((name_distance*) a);
    name_distance* nd2 = ((name_distance*) b);

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

    inputFile = f;

    // dir traversal, 8 dir aperte max
    int res = ftw(dir, add_file, 8);
    if (res != 0)
    {
        // error
    }

    // find min of list
    int min = min_list_file_distance(list);

    // filter list in place
    node* filtered = filter_list(list, EQUAL_TO, min, compare_fun);

    traverse_list(filtered, (callback_t) print_node_name);

    // frees up the list
    dispose(filtered);

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

    inputFile = (char*) f;

    // dir traversal, 8 dir aperte max
    int res = ftw(dir, add_file, MAX_OPEN_FD);
    if (res != 0)
    {
        // error
    }

    // filter list in place
    node* filtered = filter_list(list, EQ_LESS_THAN, limit, compare_fun);

    int len = count(filtered);
    if (len <= 0)
        return 0;

    name_distance* arr = NULL;

    save_to_array(filtered, &arr);

    dispose(filtered);

    qsort(arr, len, sizeof(name_distance), cmpfunc);

    for (int i = 0; i < len; i++)
    {
        print_fd(arr[i]);
    }

    return 0;
}