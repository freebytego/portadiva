#ifndef CORE_LIST_H
#define CORE_LIST_H

#include "stdlib.h"
#include "stdio.h"

typedef struct list_node_t
{
    void* data;
    struct list_node_t* next;
    struct list_node_t* previous;
} list_node_t;

typedef struct list_t
{
    unsigned int length;
    list_node_t* begin;
    list_node_t* end;
} list_t;

int create_node_list(list_t** out);
int add_node_to_node_list(list_t* list, void* data);
void remove_node_from_node_list(list_t* list, list_node_t* node); // doesn't free inside data!
void free_node_list(list_t* list); // doesn't free inside data!

#endif