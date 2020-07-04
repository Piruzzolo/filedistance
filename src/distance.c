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
#include <string.h>

#include "../include/util.h" // minmin, min, swap_array_int

#define BUFSIZE 256


int levenshtein_dist(const char* str1, size_t len1, const char* str2, size_t len2) // todo: handle null sizes
{
    if (len1 < len2)
    {
        return levenshtein_dist(str2, len2, str1, len1);
    }

    int distance = 0;

    int* prev = malloc((len2 + 1) * sizeof(int));
    if (!prev) return -1;

    int* curr = malloc((len2 + 1) * sizeof(int));
    if (!curr) return -1;
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

// CHECKED SAFE
int levenshtein_file_distance(const char* file1, const char* file2)
{
    FILE* f1 = fopen(file1, "r" );
    FILE* f2 = fopen(file2, "r" );

    int dist = 0;

    char buffer1[BUFSIZE];
    char buffer2[BUFSIZE];

    if (f1 != NULL && f2 != NULL)
    {
        while (fgets(buffer1, BUFSIZE, f1) && fgets(buffer2, BUFSIZE, f2))
        {
            dist += levenshtein_dist(buffer1, strlen(buffer1), buffer2, strlen(buffer2));
        }
    }
    else
    {
        return -1;
    }

    if (f1) fclose(f1);
    if (f2) fclose(f2);

    return dist;

}



