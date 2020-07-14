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

#ifndef DISTANCE_H
#define DISTANCE_H

#include <stdio.h>

/// Finds the Levenshtein distance between str1 and str2
///
/// \param str1 the first string
/// \param str2 the second string
/// \return
int levenshtein_dist(const char* str1, size_t len1, const char* str2, size_t len2);

int levenshtein_file_distance(const char* file1, const char* file2);


#endif // DISTANCE_H
