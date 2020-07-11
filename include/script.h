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

#ifndef FILEDISTANCE_SCRIPT_H
#define FILEDISTANCE_SCRIPT_H

#include <stdio.h> // size_t

#include "../include/distance.h"

typedef enum
{
    NONE,
    DEL,
    SET,
    ADD
} edit_type;

typedef struct _edit
{
    edit_type operation;
    unsigned int pos;
    unsigned int score;
    char c;
    struct _edit* prev;
} edit;

void print_edit(const edit* e, FILE* outfile);

int levenshtein_distance_script(const char* str1, size_t len1, const char* str2, size_t len2, edit** script);

int levenshtein_file_distance_script(const char* file1, const char* file2, const char* outfile);

#endif // FILEDISTANCE_SCRIPT_H
