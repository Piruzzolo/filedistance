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
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "../include/util.h" // minmin, min

#define BUFSIZE 256

#define MB 1024*1024 // bytes

#define MAX_MAP 10*MB

int levenshtein_dist(const char* str1, size_t len1, const char* str2, size_t len2)
{
    if (len1 == 0) return len2;

    if (len2 == 0) return len1;

    if (len1 < len2)
    {
        return levenshtein_dist(str2, len2, str1, len1);
    }

    int distance = 0;

    int* prev = calloc((len2 + 1), sizeof(int));
    int* curr = calloc((len2 + 1), sizeof(int));
    if (!curr || !prev)
        return -1;

    int* tmp = NULL;

    for (int i = 0; i <= len2; i++)
    {
        prev[i] = i;
    }

    for (int i = 1; i <= len1; i++)
    {
        curr[0] = i;

        for (int j = 1; j <= len2; j++)
        {
            if (str1[i - 1] != str2[j - 1])
            {
                int k = minmin(curr[j - 1],
                               prev[j - 1],
                                  prev[j]);
                curr[j] = k + 1;
            }
            else
            {
                curr[j] = prev[j - 1];
            }
        }

        tmp = prev;
        prev = curr;
        curr = tmp;

        memset((void*) curr, 0, sizeof(int) * (len2 + 1));
    }

    distance = prev[len2];

    free(curr);
    free(prev);

    return distance;
}


int levenshtein_file_distance(const char* file1, const char* file2)
{
    char* buf1 = NULL;
    char* buf2 = NULL;

    int dist = 0;

    struct stat st1;
    stat(file1, &st1);
    int size1 = st1.st_size;

    struct stat st2;
    stat(file2, &st2);
    int size2 = st2.st_size;

    int f1 = open(file1, O_RDONLY);
    int f2 = open(file2, O_RDONLY);

    if (f1 != -1 && f2 != -1)
    {
        buf1 = mmap(NULL, size1, PROT_READ, MAP_PRIVATE, f1,0);
        buf2 = mmap(NULL, size2, PROT_READ, MAP_PRIVATE, f2,0);

        dist = levenshtein_dist(buf1, size1, buf2, size2);
    }
    else
    {
        return -1;
    }

    munmap(buf1, size1);
    munmap(buf2, size2);

    close(f1);
    close(f2);

    return dist;
}