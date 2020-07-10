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

#include <stdlib.h> // malloc, free
#include <string.h>

#include "../include/script.h"
#include "../include/util.h"
#include "../include/endianness.h"

#define BUFSIZE 256

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
            char b = e->arg2;
            fwrite(op, sizeof(char), 3, outfile);
            fwrite(&n, sizeof(unsigned int), 1, outfile);
            fwrite(&b, 1, 1, outfile);
            break;
        }
        case DEL:
        {
            const char op[] = "DEL";
            unsigned int n = htonl(e->pos);
            fwrite(op, sizeof(char), 3, outfile);
            fwrite(&n, sizeof(unsigned int), 1, outfile);
            break;
        }
        case SET:
        {
            char op[] = "SET";
            unsigned int n = htonl(e->pos);
            char b = e->arg2;
            fwrite(op, sizeof(char), 3, outfile);
            fwrite(&n, sizeof(unsigned int), 1, outfile);
            fwrite(&b, 1, 1, outfile);
            break;
        }
        default:
            return;
    }
}


unsigned int levenshtein_matrix_calculate(edit** mat, const char* str1, size_t len1, const char *str2, size_t len2)
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

            del = mat[i - 1][j].score + 1; /* deletion */
            ins = mat[i][j - 1].score + 1; /* insertion */
            subst = mat[i - 1][j - 1].score + substitution_cost; /* substitution */

            best = minmin(del, ins, subst);

            mat[i][j].score = best;
            mat[i][j].arg1 = str1[i - 1];
            mat[i][j].arg2 = str2[j - 1];
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

edit** levenshtein_matrix_create(size_t len1, size_t len2)
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
            return NULL;
        }
    }
    for (int i = 0; i <= len1; i++)
    {
        mat[i][0].score = i;
        mat[i][0].prev = NULL;
        mat[i][0].arg1 = 0;
        mat[i][0].arg2 = 0;
    }

    for (int j = 0; j <= len2; j++)
    {
        mat[0][j].score = j;
        mat[0][j].prev = NULL;
        mat[0][j].arg1 = 0;
        mat[0][j].arg2 = 0;
    }

    return mat;
}

int levenshtein_distance_script(const char* str1, size_t len1, const char* str2, size_t len2, edit** script)
{
    unsigned int distance;

    edit** mat = NULL;
    edit* head = NULL;

    /* If either string is empty, the distance is the other string's length */
    if (len1 == 0)
    {
        return len2;
    }
    if (len2 == 0)
    {
        return len1;
    }
    /* Initialise the matrix */
    mat = levenshtein_matrix_create(len1, len2);
    if (!mat)
    {
        *script = NULL;
        return 0;
    }
    /* Main algorithm */
    distance = levenshtein_matrix_calculate(mat, str1, len1, str2, len2);
    /* Read back the edit script */
    *script = malloc(distance * sizeof(edit));

    if (*script)
    {
        unsigned int i = distance - 1;
        for (head = &mat[len1][len2]; head->prev != NULL; head = head->prev)
        {
            if (head->operation != NONE)
            {
                memcpy(*script + i, head, sizeof(edit));
                i--;
            }
        }
    }
    else
    {
        distance = 0;
    }

    /* Clean up */
    for (int i = 0; i <= len1; i++)
    {
        free(mat[i]);
    }

    free(mat);

    return distance;
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
    FILE* f1 = fopen(file1, "r" );
    FILE* f2 = fopen(file2, "r" );
    FILE* out = fopen(outfile, "w+");

    char buffer1[BUFSIZE];
    char buffer2[BUFSIZE];

    edit* script = NULL;

    int dist = 0;

    if (f1 != NULL && f2 != NULL)
    {
        while (fgets(buffer1, BUFSIZE, f1) && fgets(buffer2, BUFSIZE, f2))
        {
            int distance = 0;

            if (strlen(buffer1) < strlen(buffer2))
            {
                distance = levenshtein_distance_script(buffer2, strlen(buffer2), buffer1, strlen(buffer1), &script);
            }
            else
            {
                distance = levenshtein_distance_script(buffer1, strlen(buffer1), buffer2, strlen(buffer2), &script);
            }

            append_script_file(out, script, distance);
            dist += distance;
        }
    }
    else
    {
        return -1;
    }

    printf("Distance: %d\n", dist);

    printf("Edit script saved successfully: %s\n", outfile);

    if (f1) fclose(f1);
    if (f2) fclose(f2);
    if (out) fclose(out);

    return 0;

}

