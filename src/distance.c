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

#include "../include/distance.h"
#include "../include/script.h"
#include "../include/util.h"

int levdistOpt(char* str1, size_t len1, char* str2, size_t len2)
{
    int* cost    = calloc(len1, sizeof(int));
    int* newcost = calloc(len1, sizeof(int));

    if (!cost || !newcost)
    {
        return 1;
    }

    // initial cost of skipping prefix in String s0
    for (int i = 0; i < len1; i++)
    {
        cost[i] = i;
    }

    // dynamically computing the array of distances

    // transformation cost for each letter in s1
    for (int j = 1; j < len2; j++)
    {
        // initial cost of skipping prefix in String s1
        newcost[0] = j;

        // transformation cost for each letter in s0
        for (int i = 1; i < len1; i++)
        {
            // matching current letters in both strings
            int match = (str1[i - 1] == str2[j - 1]) ? 0 : 1;

            // computing cost for each transformation
            int cost_replace = cost[i - 1] + match;
            int cost_insert = cost[i] + 1;
            int cost_delete = newcost[i - 1] + 1;

            // keep minimum cost
            newcost[i] = minmin(cost_insert,
                                cost_delete,
                                cost_replace);

        }

        swap_array_int(cost, newcost, len1);

    }

    // the distance is the cost for transforming all letters in both strings
    int d = cost[len1 - 1];
    
    free(cost);
    free(newcost);

    return d;
}

int fileDistance(char* file1, char* file2)
{
    char* buff1;
    char* buff2;

    int size1 = 0;
    int size2 = 0;

    size1 = loadFile(file1, &buff1);
    size2 = loadFile(file2, &buff2);

    if (size1 < 0 || size2 < 0 ||
        buff1 == NULL || buff2 == NULL)
    {
        return -1;
    }

    int distance = 0;

    distance = levdistOpt(buff1, size1, buff2, size2);

    free(buff1);
    free(buff2);

    return distance;
}



