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
#include <ftw.h>

#include "../include/search.h"
#include "../include/list.h"
#include "../include/script.h"
#include "../include/safe_str/strlcpy.h"

char* inputFile;
static node* list = NULL;
FilenameDistance* fdList = NULL;

int printDistances(const char *fname, const struct stat *st, int type)
{
    if (type != FTW_F)
        return 0;

    int distance = fileDistance(fname, inputFile);

    FilenameDistance* fd = (FilenameDistance*) malloc(sizeof(FilenameDistance));
    if (!fd) return -1;

    fd->distance = distance;

    // copia e rileva il troncamento
    if (strlcpy(fd->filename, fname, sizeof(fd->filename)) >= sizeof(fd->filename))
    {
        // errore?
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

void printNode(node* node)
{
    FilenameDistance* fd = ((FilenameDistance*) node->data);

    char* filename = fd->filename;
    int distance = fd->distance;
    printf("%s -> %d\n", filename, distance);
}

void saveArray(node* node)
{
    FilenameDistance* fd = ((FilenameDistance*) node->data);

    char* filename = fd->filename;
    int distance = fd->distance;
    printf("%s -> %d\n", filename, distance);

}

int searchPrint(char* f, char* dir)
{
    if (f == NULL || dir == NULL)
    {
        return -1;
    }

    inputFile = f;

    // dir traversal, 8 dir aperte max
    ftw(dir, printDistances, 8);

    traverse(list, (callback_t) printNode);

    int num = count(list);
    if (num > 0)
    {
       fdList = (FilenameDistance*) malloc(num * sizeof(FilenameDistance));
    }

    // callback c = saveArray;
    // traverse(list, c); todo

    return 0;

}

char** searchAll(FILE* inputfile, DIR* dir, int limit);
