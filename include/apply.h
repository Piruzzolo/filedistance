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

#ifndef APPLY_H
#define APPLY_H

#include <stdio.h>


enum {
    EEMPTYSCRIPT,
    ECANTOPEN,
    ECORRUPTD
} applyErr_t;


/// Prints err in text form
///
/// \param err the err to print
void apply_print_err(int err);


/// Applies the filem edits to infile, outputting to outfile
///
/// \param infile the file to apply to
/// \param filem edit script with commands
/// \param outfile the file to save to
/// \return 0 if succeeds, -1 if err. Sets errno
int apply_edit_script(const char* infile, const char* filem, const char* outfile);


#endif // APPLY_H
