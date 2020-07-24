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
#include <sys/stat.h> // stat
#ifdef MMAP
    #include <fcntl.h>    // open
    #include <unistd.h>   // close
    #include <sys/mman.h> // MMAP
#endif

#include "../include/util.h" // min, minmin


int distance_string(const char* str1, size_t len1, const char* str2, size_t len2)
{
    if (len1 == 0)
        return len2;

    if (len2 == 0)
        return len1;

    if (len1 < len2)
    {
        return distance_string(str2, len2, str1, len1);
    }

    int distance = 0;

    /* allocate prev and curr rows */
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
            /* Wagner-Fischer, keeping best cost */
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

        /* swap arrays */
        tmp = prev;
        prev = curr;
        curr = tmp;

        /* reset curr */
        memset((void*) curr, 0, sizeof(int) * (len2 + 1));
    }

    distance = prev[len2];

    /* free & avoid double free */
    free(curr);
    free(prev);
    curr = NULL;
    prev = NULL;

    return distance;
}

int distance_file(const char* file1, const char* file2)
{
    char* buf1 = NULL;
    char* buf2 = NULL;

    if (file1 == NULL || file2 == NULL)
    {
        return -1;
    }

    int dist = 0;

    /* get size of files */

    struct stat st1;
    stat(file1, &st1);
    int size1 = st1.st_size;

    struct stat st2;
    stat(file2, &st2);
    int size2 = st2.st_size;

#ifdef MMAP
    /* open files read only */
    int f1 = open(file1, O_RDONLY);
    int f2 = open(file2, O_RDONLY);

    if (f1 != -1 && f2 != -1)
    {
        /* memory-map files */
        buf1 = mmap(NULL, size1, PROT_READ, MAP_PRIVATE, f1,0);
        buf2 = mmap(NULL, size2, PROT_READ, MAP_PRIVATE, f2,0);

        /* give kernel some hints on usage pattern */
        madvise(buf1, size1, MADV_SEQUENTIAL);
        madvise(buf2, size2, MADV_SEQUENTIAL);

        /* find distance */
        dist = distance_string(buf1, size1, buf2, size2);
    }
    else
    {
        return -1;
    }

    /* unmap files */
    munmap(buf1, size1);
    munmap(buf2, size2);

    close(f1);
    close(f2);

    return dist;
#else

    /* load files to buffers and find distance */

    if (file_load(file1, &buf1) && file_load(file2, &buf2))
    {
        dist = distance_string(buf1, size1, buf2, size2);

        /* free buffers */
        free(buf1);
        free(buf2);
        buf1 = NULL;
        buf2 = NULL;

        return dist;
    }
    else
    {
        return -1;
    }

#endif
}