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

#ifndef FILEDISTANCE_SCRIPT_H
#define FILEDISTANCE_SCRIPT_H

#include <stdio.h> // size_t

typedef enum
{
    NONE,
    DELETION,
    SUBSTITUTION,
    INSERTION
} edit_type;

struct _edit
{
    edit_type type;
    unsigned int pos;
    unsigned int score;
    char arg1;
    char arg2;
    struct _edit* prev;
};

typedef struct _edit edit;

int fileDistance(char* file1, char* file2);

int levenshtein_distance(char* str1, char* str2, edit** script);
int levenshtein_matrix_calculate(edit* script, char* str1, int m, char* str2, int n);

#endif // FILEDISTANCE_SCRIPT_H