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

#ifndef FILEDISTANCE_LIST_H
#define FILEDISTANCE_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "search.h"
#include "safestr.h"

typedef struct node
{
    void* data;
    struct node* next;
} node;

typedef enum
{
    EQUAL_TO,
    LESS_THAN,
    GTR_THAN,
    EQ_LESS_THAN,
    EQ_GTR_THAN
} op_t;


/* callbacks and function with defined prototypes,
 * just for convenience */

typedef void (*callback_t)(node* data);
typedef bool (*comparison_f)(void* data, op_t op, long value);

/// Creates a new list
///
/// \param data the payload
/// \param next the next node
/// \return a pointer to the node created
node* list_create(void* data, node* next);


/// Appends an element to list
///
/// \param list the list
/// \param data the payload
/// \return a pointer to the node created
node* list_append(node* list, void* data);


/// Filters a list with the function f
///
/// \param list
/// \param op
/// \param value
/// \param f
/// \return
node* list_filter(node* list, comparison_f f, int op, long value);


/// Traverses the list applying f at each node
///
/// \param list the list to traverse
/// \param f callback to apply
void list_traverse(node* list, callback_t f);


/// Deallocates the list
///
/// \param list the list to be deallocated
void list_free(node* list);


/// Counts the items of the list
///
/// \param list the list to search into
/// \return the count of the items
int list_count(node* list);


#endif //FILEDISTANCE_LIST_H
