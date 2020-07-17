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

#define KB 1024
#define BUFSIZE 10*KB


int manhattanDistance(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

// per la derivazione vedere la documentazione
_Bool isOutCell(i, j, n, m)
{
    return 2 * min(manhattanDistance(i, j, 0, n),manhattanDistance(i, j, m, 0)) <= n;
}

// conta il numero di celle valide per una matrice m x n
int count_valid_cells(int m, int n)
{
    int count = 0;
    for (int i = 0; i <= m; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (isOutCell(i, j, n, m))
            {
                count++;
            }
        }
    }
    return count;
}


void print_edit(const edit* e, FILE* outfile)
{
    switch (e->operation)
    {
        case ADD:
        {
            const char op[] = "ADD";
            unsigned int n = htonl(e->pos);
            char b = e->c;
            fwrite(op, sizeof(char), sizeof(op) - 1, outfile);
            fwrite(&n, sizeof(unsigned int), 1, outfile);
            fwrite(&b, 1, 1, outfile);
            break;
        }
        case DEL:
        {
            const char op[] = "DEL";
            unsigned int n = htonl(e->pos);
            fwrite(op, sizeof(char), sizeof(op) - 1, outfile);
            fwrite(&n, sizeof(unsigned int), 1, outfile);
            break;
        }
        case SET:
        {
            const char op[] = "SET";
            unsigned int n = htonl(e->pos);
            char b = e->c;
            fwrite(op, sizeof(char), sizeof(op) - 1, outfile);
            fwrite(&n, sizeof(unsigned int), 1, outfile);
            fwrite(&b, 1, 1, outfile);
            break;
        }

        default:
            return;
    }
}


unsigned int levenshtein_fill_matrix(edit** mat, const char* str1, size_t len1, const char *str2, size_t len2)
{
    unsigned int i, j;
    for (j = 1; j <= len2; j++)
    {
        for (i = 1; i <= len1; i++)
        {
            //if (isOutCell(j, i, len2, len1))
            //    continue;

            unsigned int substitution_cost;
            unsigned int del = 0, ins = 0, subst = 0;
            unsigned int best;

            if (str1[i - 1] == str2[j - 1])
            {
                substitution_cost = 0;
            }
            else
            {
                substitution_cost = 1;
            }

            del = mat[i - 1][j].score + 1;
            ins = mat[i][j - 1].score + 1;
            subst = mat[i - 1][j - 1].score + substitution_cost;

            best = minmin(del, ins, subst);

            mat[i][j].score = best;
            mat[i][j].c = str2[j - 1];
            mat[i][j].pos = i - 1;

            if (best == del)
            {
                mat[i][j].operation = DEL;
                mat[i][j].prev = &mat[i - 1][j];
            }
            else if (best == ins)
            {
                mat[i][j].operation = ADD;
                mat[i][j].prev = &mat[i][j - 1];
            }
            else
            {
                if (substitution_cost > 0)
                {
                    mat[i][j].operation = SET;
                }
                else
                {
                    mat[i][j].operation = NONE;
                }
                mat[i][j].prev = &mat[i - 1][j - 1];
            }
        }
    }

    return mat[len1][len2].score;
}

edit** levenshtein_create_matrix(size_t len1, size_t len2)
{
    edit** mat = malloc((len1 + 1) * sizeof(edit*));
    if (mat == NULL)
    {
        return NULL;
    }

    for (int i = 0; i <= len1; i++)
    {
        mat[i] = malloc((len2 + 1) * sizeof(edit));
        if (mat[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(mat[j]);
            }

            free(mat);
            mat = NULL;
            return NULL;
        }
    }

    for (int i = 0; i <= len1; i++)
    {
        mat[i][0].score = i;
        mat[i][0].prev = NULL;
        mat[i][0].c = 0;
    }

    for (int j = 0; j <= len2; j++)
    {
        mat[0][j].score = j;
        mat[0][j].prev = NULL;
        mat[0][j].c = 0;
    }

    return mat;
}

int levenshtein_distance_script(const char* str1, size_t len1, const char* str2, size_t len2, edit** script)
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
        unsigned int i = dist - 1;
        edit* curr;
        for (curr = &mat[len1][len2]; curr->prev != NULL; curr = curr->prev)
        {
            if (curr->operation == NONE)
            {
                continue;
            }

            memcpy(*script + i, curr, sizeof(edit));
            i--;
        }
    }

    /* free matrix */
    for (int i = 0; i <= len1; i++)
    {
        free(mat[i]);
    }

    free(mat);
    mat = NULL;

    return dist;
}


void append_script_file(FILE* file, edit* script, size_t len)
{
    for (int i = 0; i < len; i++)
    {
        print_edit(&script[i], file);
    }
}

int levenshtein_file_distance_script(const char* file1, const char* file2, const char* outfile)
{
    char buffer1[BUFSIZE];
    char buffer2[BUFSIZE];

    edit* script = NULL;

    int tot = 0;

    FILE* f1 = fopen(file1, "r" );
    FILE* f2 = fopen(file2, "r" );
    FILE* out = fopen(outfile, "w");

    if (f1 != NULL && f2 != NULL)
    {
        while (fgets(buffer1, BUFSIZE, f1) && fgets(buffer2, BUFSIZE, f2))
        {
            int distance = 0;

            //if (strlen(buffer1) < strlen(buffer2))
            //{
            distance = levenshtein_distance_script(buffer2, strlen(buffer2), buffer1, strlen(buffer1), &script);
            //}
           // else
           // {
                //distance = levenshtein_distance_script(buffer1, strlen(buffer1), buffer2, strlen(buffer2), &script);
           // }

            append_script_file(out, script, distance);
            tot += distance;
        }
    }
    else
    {
        return -1;
    }

    printf("Distance: %d\n", tot);

    printf("Edit script saved successfully: %s\n", outfile);

    if (f1) fclose(f1);
    if (f2) fclose(f2);
    if (out) fclose(out);

    return 0;

}