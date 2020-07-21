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

#ifndef FILEDISTANCE_UTIL_H
#define FILEDISTANCE_UTIL_H

#include <stdio.h>
#include "script.h"


/// Calculates min between x and y
///
/// \param x first integer
/// \param y second integer
/// \return the min
int min(int x, int y);


/// Calculates the min between 3 values
///
/// \param x first integer
/// \param y second integer
/// \param z third integer
/// \return the min
int minmin(int x, int y, int z);


/// Copies len bytes from in to out, starting off from their current seek
///
/// \param in the file to copy from
/// \param out the file to copy to
/// \param len amount of bytes to copy
/// \return 0 if succeeded, -1 otherwise
int file_copy(FILE* in, FILE* out, unsigned int len);


/// Converts buf to unsigned int 32 bit
///
/// \param buf the char array to convert
/// \return the converted unsigned value
u_int32_t bytes_to_uint32(char* buf);

#endif // FILEDISTANCE_UTIL_H
