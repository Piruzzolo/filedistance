/* This file is part of FileDistance
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

typedef struct node
{
    void* data;
    struct node* next;
} node;

enum
{
    EQUAL_TO,
    LESS_THAN,
    GTR_THAN,
    EQ_LESS_THAN,
    EQ_GTR_THAN
} op_t;

typedef void (*callback_t)(node* data);

typedef _Bool (*comparison_f)(void* data, int op, int value);

/*
    create a new node
    initialize the data and next field

    return the newly created node
*/
node* create(void* data, node* next);

void MergeSort(node* head);
/*
    add a new node at the beginning of the list
*/
node* prepend(node* head, void* data);

/*
    add a new node at the end of the list
*/
node* append(node* head, void* data);

/*
    insert a new node after the prev node
*/
node* insert_after(node *head, void* data, node* prev);

/*
    insert a new node before the nxt node
*/
node* insert_before(node *head, void* data, node* nxt);

node* filter_list(node* head, int op, long value, comparison_f f);

/*
    traverse the linked list
*/
void traverse_list(node* head, callback_t f);
/*
    remove node from the front of list
*/
node* remove_front(node* head);

/*
    remove node from the back of the list
*/
node* remove_back(node* head);

/*
    remove a node from the list
*/
node* remove_any(node* head,node* nd);
/*
    display a node
*/
void display(node* n);

/*
    Search for a specific node with input data

    return the first matched node that stores the input data,
    otherwise return NULL
*/
//node* search(node* head, void* data);

/*
    remove all element of the list
*/
void dispose(node *head);
/*
    return the number of elements in the list
*/
int count(node *head);
/*
    sort the linked list using insertion sort
*/
node* insertion_sort(node* head);

/*
    reverse the linked list
*/
node* reverse(node* head);
/*
    display the menu
*/

#endif //FILEDISTANCE_LIST_H
