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

#ifndef FILEDISTANCE_STRLCPY_H
#define FILEDISTANCE_STRLCPY_H


/// Size-bounded string copying and concatenation
///
/// \param dst destination string
/// \param src source string
/// \param dsize len to copy
/// \return total length of the string tried to create
size_t strlcpy(char * dst, const char * src, size_t dsize);


#endif //FILEDISTANCE_STRLCPY_H
