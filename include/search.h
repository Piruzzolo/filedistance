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

#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <dirent.h>


/// Search files in dir (and subdirs) with distance from inputfile <= limit,
/// printing them to stdout sorted by length ascending, filename ascending
///
/// \param inputfile the file to compare against
/// \param dir the directory to traverse
/// \param limit the limit on the distance
/// \return 0 if succeeded, -1 otherwise
int search_all(const char* inputfile, const char* dir, long limit);

/// Search files in dir (and subdirs) with distance from inputfile == limit,
/// printing them to stdout sorted by length ascending, filename ascending
///
/// \param inputfile the file to compare against
/// \param dir the directory to traverse
/// \param limit the limit on the distance
/// \return 0 if succeeded, -1 otherwise
int search_min(const char* filename, const char* dir);

#endif //UNTITLED_SEARCH_H
