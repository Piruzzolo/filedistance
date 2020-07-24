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
    NOP,
    DEL,
    SET,
    ADD
} op_type;


/* edit struct is byte-aligned */
typedef struct _edit
{
    op_type operation     : 2;
    unsigned int score    : 14;
    unsigned int position : 16;
    char c;
} edit;


/// Saves e binarily to a file according to the prescribed format
///
/// \param e the edit to save
/// \param outfile the file to save to
void script_print_edit(const edit* e, FILE* outfile);


/// Finds the minimal edit script and distance between two strings
///
/// \param str1 first string
/// \param len1 len of first string
/// \param str2 second string
/// \param len2 length of second string
/// \param script edit script to save
/// \return the distance
int script_string_distance(const char* str1, size_t len1, const char* str2, size_t len2, edit** script);


/// Finds the minimal edit script and distance between two files, saving script it to outfile
///
/// \param file1 first file
/// \param file2 second file
/// \param outfile file to save to
/// \return the distance
int script_file_distance(const char* file1, const char* file2, const char* outfile);


#endif // FILEDISTANCE_SCRIPT_H
