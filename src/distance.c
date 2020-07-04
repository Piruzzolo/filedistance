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

int levenshtein_dist(const char* x, size_t m, const char* y, size_t n)
{
    int distance;

    int* prev = malloc((n + 1) * sizeof(int));
    int* curr = malloc((n + 1) * sizeof(int));
    int* tmp = 0;

    for(int i = 0; i <= n; i++)
        prev[i] = i;

    for(int i = 1; i <= m; i++)
    {
        curr[0] = i;

        for(int j = 1; j <= n; j++)
        {
            if(x[i - 1] != y[j - 1])
            {
                int k = minmin(curr[j - 1], prev[j - 1], prev[j]);
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

        memset((void*) curr, 0, sizeof(int) * (n + 1));
    }

    distance = prev[n];

    free(curr);
    free(prev);

    return distance;
}


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

    fclose(f1);
    fclose(f2);

    return dist;

}



