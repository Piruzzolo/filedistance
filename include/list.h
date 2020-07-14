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

typedef void (*callback_t)(node* data);

typedef bool (*comparison_f)(void* data, long op, int value);

node* list_create(void* data, node* next);

node* list_append(node* head, void* data);

node* list_filter(node* head, int op, long value, comparison_f f);

/*
    traverse the linked list
*/
void list_traverse(node* head, callback_t f);

void list_free(node *head);
/*
    return the number of elements in the list
*/
int list_count(node *head);


#endif //FILEDISTANCE_LIST_H
