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

int node_list_create(list_t** out);
int node_list_add_node(list_t* list, void* data);
void node_list_remove_node(list_t* list, list_node_t* node); // doesn't free inside data!
void node_list_free(list_t* list); // doesn't free inside data!
void node_list_reset(list_t* list);

#endif