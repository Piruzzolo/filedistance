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

#ifndef FILEDISTANCE_NAME_DISTANCE_H
#define FILEDISTANCE_NAME_DISTANCE_H

#include <limits.h>

typedef struct _name_distance
{
    int distance;
    char filename[PATH_MAX];
} name_distance;


void namedistance_print(name_distance nd);


#endif //FILEDISTANCE_NAME_DISTANCE_H
