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

#ifndef FILEDISTANCE_LIST_NAMEDISTANCE_H
#define FILEDISTANCE_LIST_NAMEDISTANCE_H

#include "list.h"
#include "name_distance.h"

enum {
    EEMPTYLIST
} list_err_t;


/// Save the list to the array arr
/// \param list the list to save
/// \param arr the array
/// \return an error code
int list_namedistance_save_to_array(node* list, name_distance** arr);

/// Find min of distances
/// \param list the list to go through
/// \return the min distance found
int list_namedistance_min(node* list);


void list_namedistance_print_name(node* node);

#endif //FILEDISTANCE_LIST_NAMEDISTANCE_H
