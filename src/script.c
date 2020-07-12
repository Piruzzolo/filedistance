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
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>

#include "../include/script.h"
#include "../include/util.h"
#include "../include/endianness.h"

#define BUFSIZE 256


void print_edit(const edit* e, FILE* outfile)
{
    switch (e->operation)
    {
        case ADD:
        {
            const char op[] = "ADD";
            unsigned int n = htonl(e->pos);
            char b = e->c;
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
            const char op[] = "SET";
            unsigned int n = htonl(e->pos);
            char b = e->c;
            fwrite(op, sizeof(char), 3, outfile);
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
    for (int j = 1; j <= len2; j++)
    {
        for (int i = 1; i <= len1; i++)
        {
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

            del = mat[i - 1][j].score + 1;                       /* deletion */
            ins = mat[i][j - 1].score + 1;                       /* insertion */
            subst = mat[i - 1][j - 1].score + substitution_cost; /* substitution */

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

edit** levenshtein_matrix_create(size_t len1, size_t len2, int fd)
{
    long size = (len2 + 1) * (len1 + 1) * sizeof(edit);
    edit (*mat)[len2 + 1] = (edit (*)[len2 + 1]) mmap(NULL,
                                              size,
                                                  PROT_WRITE | PROT_READ,
                                                 MAP_SHARED,
                                                 fd,
                                             0);

    if (mat == MAP_FAILED)
    {
        return NULL;
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

void backtrace(edit (*mat)[], int i, int j)
{
    if (i == 0 || j == 0)
    {
        return;
    }

    //if (mat[i][j].operation == SET)
    //{
    //    backtrace(mat, i-1, j-1);
    //    printf("xx\n");
    //}
    //else if (mat[i][j].operation == ADD)
    //{
    //    backtrace(mat, i-1, j);
    //}
    //else
    //{
    //    backtrace(mat, i, j-1);
    //}
}

// Function to reverse elements of an array
void reverse(int arr[], int n)
{
    int aux[n];

    for (int i = 0; i < n; i++) {
        aux[n - 1 - i] = arr[i];
    }

    for (int i = 0; i < n; i++) {
        arr[i] = aux[i];
    }
}


int levenshtein_distance_script(const char* str1, size_t l1, const char* str2, size_t l2, edit** script)
{
    if (l1 < l2)
    {
        return levenshtein_distance_script(str2, l2, str1, l1, script);
    }

    unsigned int dist;

    /* create a temp file and get descriptor */
    int fd = fileno(tmpfile());

    /* set size of tmpfile */
    ftruncate(fd, (l1 + 1) * (l2 + 1) * sizeof(edit));

    /* degenerate cases */
    if (l1 == 0)
    {
        return l2;
    }
    if (l2 == 0)
    {
        return l1;
    }

    long size = (l2 + 1) * (l1 + 1) * sizeof(edit);
    edit (*mat)[l2 + 1] = (edit (*)[l2 + 1]) mmap(NULL,
                                                      size,
                                                      PROT_WRITE | PROT_READ,
                                                      MAP_SHARED,
                                                      fd,
                                                      0);
    //madvise(mat, size, MADV_SEQUENTIAL);

    if (mat == MAP_FAILED)
    {
        return NULL;
    }

    for (int i = 0; i <= l1; i++)
    {
        mat[i][0].score = i;
        mat[i][0].prev = NULL;
        mat[i][0].c = 0;
    }

    for (int j = 0; j <= l2; j++)
    {
        mat[0][j].score = j;
        mat[0][j].prev = NULL;
        mat[0][j].c = 0;
    }

    if (mat == NULL)
    {
        *script = NULL;
        return 0;
    }

    for (int j = 1; j <= l2; j++)
    {
        for (int i = 1; i <= l1; i++)
        {
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

            del = mat[i - 1][j].score + 1;                       /* deletion */
            ins = mat[i][j - 1].score + 1;                       /* insertion */
            subst = mat[i - 1][j - 1].score + substitution_cost; /* substitution */

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

    dist = mat[l1][l2].score;


    /* Read back the edit script */
    *script = malloc(dist * sizeof(edit));

    if (!(*script))
    {
        dist = 0;
    }
    else
    {
        unsigned int i = dist - 1;
        edit* head = NULL;
        for (head = &mat[l1][l2]; head->prev != NULL; head = head->prev)
        {
            if (head->operation == NONE)
                continue;

            memcpy(*script + i, head, sizeof(edit));
            i--;
        }
//


       //unsigned int i = l1;
       //unsigned int j = l2;
       //unsigned int index = dist - 1;

       //for (int cnt = 0; cnt < dist; cnt++)
       //{
       //    if (mat[i][j].operation == DEL)
       //    {
       //        memcpy(*script + index, &mat[i][j], sizeof(edit));
       //        index--;
       //        //printf("DEL\n");
       //        i = i-1;
       //    }
       //    else if (mat[i][j].operation == ADD)
       //    {
       //        memcpy(*script + index, &mat[i][j], sizeof(edit));
       //        index--;
       //        //printf("ADD\n");
       //        j = j-1;
       //    }
       //    else if (mat[i][j].operation == SET)
       //    {
       //        memcpy(*script + index, &mat[i][j], sizeof(edit));
       //        index--;
       //        //printf("SET\n");
       //        i = i-1;
       //        j = j-1;
       //    }
       //    else // NONE
       //    {
       //        ss++;
       //        i = i-1;
       //        j = j-1;
       //    }
//////
       //}


    }

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

    int dist = 0;

    FILE* f1 = fopen(file1, "r" );
    FILE* f2 = fopen(file2, "r" );
    FILE* out = fopen(outfile, "w");

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

    fclose(f1);
    fclose(f2);
    fclose(out);

    return 0;

    //char* buffer1 = NULL;
    //char* buffer2 = NULL;
//
    //edit* script = NULL;
//
    //int dist = 0;
//
    //struct stat st1;
    //stat(file1, &st1);
    //int size1 = st1.st_size;
//
    //struct stat st2;
    //stat(file2, &st2);
    //int size2 = st2.st_size;
//
    //int f1 = open(file1, O_RDONLY);
    //int f2 = open(file2, O_RDONLY);
//
    //FILE* out = fopen(outfile, "w");
//
    //if (f1 != -1 && f2 != -1)
    //{
    //    buffer1 = mmap(NULL, size1, PROT_READ, MAP_SHARED, f1,0);
    //    buffer2 = mmap(NULL, size2, PROT_READ, MAP_SHARED, f2,0);
//
    //    dist = levenshtein_distance_script(buffer1, size1, buffer2, size2, &script);
//
    //    for (int i = 0; i < dist; i++)
    //    {
    //        print_edit(&script[i], out);
    //    }
//
    //    append_script_file(out, script, dist);
    //}
    //else
    //{
    //    return -1;
    //}
//
    //printf("Distance: %d\n", dist);
//
    //printf("Edit script saved successfully: %s\n", outfile);
//
    //munmap(buffer1, size1);
    //munmap(buffer2, size2);
//
    //close(f1);
    //close(f2);
//
    //return 0;

}

