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

#include <stdlib.h> // malloc, free
#include <string.h>

#include "../include/script.h"
#include "../include/util.h"
#include "../include/endianness.h"


void script_print_edit(const edit* e, FILE* outfile)
{
    unsigned int n = htonl(e->position);
    char b = 0;
    char* op;
    switch (e->operation)
    {
        case ADD:
        {
            op = "ADD";
            b = e->c;
            break;
        }
        case DEL:
        {
            op = "DEL";
            b = 0;
            break;
        }
        case SET:
        {
            op = "SET";
            b = e->c;
            break;
        }

        default:
            return;
    }

    fwrite(op, sizeof(char), sizeof(op) - 1, outfile);
    fwrite(&n, sizeof(unsigned int), 1, outfile);
    fwrite(&b, 1, 1, outfile);
}


unsigned int levenshtein_fill_matrix(edit** matrix, const char* str1, size_t m, const char *str2, size_t n)
{
    for (int j = 1; j <= n; j++)
    {
        for (int i = 1; i <= m; i++)
        {
            int substitution_cost;
            int del = 0, ins = 0, subst = 0;
            int best;

            if (str1[i - 1] == str2[j - 1])
            {
                substitution_cost = 0;
            }
            else
            {
                substitution_cost = 1;
            }

            del = matrix[i - 1][j].score + 1;
            ins = matrix[i][j - 1].score + 1;
            subst = matrix[i - 1][j - 1].score + substitution_cost;

            best = minmin(del, ins, subst);

            matrix[i][j].score = best;
            matrix[i][j].position = i - 1;
            matrix[i][j].c = str2[j - 1];

            if (best == del)
            {
                matrix[i][j].operation = DEL;
            }
            else if (best == ins)
            {
                matrix[i][j].operation = ADD;
            }
            else
            {
                if (substitution_cost > 0)
                {
                    matrix[i][j].operation = SET;
                }
                else
                {
                    matrix[i][j].operation = NOP;
                }
            }
        }
    }

    return matrix[m][n].score;
}


edit** levenshtein_create_matrix(size_t l1, size_t l2)
{
    edit** matrix = malloc((l1 + 1) * sizeof(edit*));
    if (matrix == NULL)
    {
        return NULL;
    }

    for (int i = 0; i <= l1; i++)
    {
        matrix[i] = malloc((l2 + 1) * sizeof(edit));
        if (matrix[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(matrix[j]);
            }

            free(matrix);
            matrix = NULL;

            return NULL;
        }
    }

    for (int i = 0; i <= l1; i++)
    {
        matrix[i][0].score = i;
        matrix[i][0].c = 0;
    }

    for (int j = 0; j <= l2; j++)
    {
        matrix[0][j].score = j;
        matrix[0][j].c = 0;
    }

    return matrix;
}


int script_string_distance(const char* str1, size_t len1, const char* str2, size_t len2, edit** script)
{
    unsigned int dist;

    if (len1 == 0)
    {
        return len2;
    }
    if (len2 == 0)
    {
        return len1;
    }

    edit** mat = levenshtein_create_matrix(len1, len2);
    if (!mat)
    {
        *script = NULL;
        return 0;
    }

    dist = levenshtein_fill_matrix(mat, str1, len1, str2, len2);

    *script = malloc(dist * sizeof(edit));
    if (!(*script))
    {
        dist = 0;
    }
    else
    {
        unsigned int p = dist - 1;
        int i = len1;
        int j = len2;
        edit* curr = &mat[i][j];
        while (i >= 0 || j >= 0)
        {
            switch (curr->operation)
            {
                case ADD:
                {
                    memcpy(*script + p, curr, sizeof(edit));
                    p--;

                    curr = &mat[i][--j];
                    break;
                }
                case DEL:
                {
                    memcpy(*script + p, curr, sizeof(edit));
                    p--;

                    curr = &mat[--i][j];
                    break;
                }
                default:
                {
                    if (curr->operation == NOP)
                    {
                        curr = &mat[--i][--j];
                        continue;
                    }
                    memcpy(*script + p, curr, sizeof(edit));
                    p--;

                    curr = &mat[--i][--j];
                    break;
                }
            }
        }
    }

    /* free matrix's inner arrays */
    for (int i = 0; i <= len1; i++)
    {
        free(mat[i]);
    }

    /* free matrix */
    free(mat);
    mat = NULL;

    return dist;
}


int append_script_file(const char* file, edit* script, size_t len)
{
    FILE* f = fopen(file, "w");
    if (f)
    {
        for (int i = 0; i < len; i++)
        {
            script_print_edit(&script[i], f);
        }
        return 0;
    }
    else
    {
        return -1;
    }
}


int script_file_distance(const char* file1, const char* file2, const char* outfile)
{
    if (file1 == NULL || file2 == NULL)
    {
        return -1;
    }

    char* buf1 = NULL;
    char* buf2 = NULL;

    int distance = 0;

    edit* script = NULL;

    if (file_load(file1, &buf1) && file_load(file2, &buf2))
    {
        distance = script_string_distance(buf1, strlen(buf1), buf2, strlen(buf2), &script);
    }
    else
    {
        return -1;
    }

    if (append_script_file(outfile, script, distance) < 0)
    {
        return -1;
    }

    free(script);
    script = NULL;

    return distance;

}