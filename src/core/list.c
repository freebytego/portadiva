#include "include/core/list.h"

int node_list_create(list_t** out)
{
    list_t* list = (list_t*)malloc(sizeof(list_t));
    if (NULL == list)
    {
        perror("failed to allocate a list\n");
        return -1;
    }
    list->length = 0;
    list->begin = NULL;
    list->end = NULL;
    *out = list;
    #if __DEBUG__
    printf("list created at %p\n", *out);
    #endif
    return 0;
}

int node_list_add_node(list_t* list, void* data)
{
    if (NULL == list)
    {
        perror("tried to insert into an empty list\n");
        return -1;
    }
    list_node_t* node = (list_node_t*)malloc(sizeof(list_node_t));
    if (NULL == node)
    {
        perror("failed allocating a node\n");
        return -1;
    }
    node->data = data;
    node->previous = list->end;
    node->next = NULL;
    
    if (NULL == list->begin) list->begin = node;
    if (NULL != list->end) list->end->next = node;
    list->end = node;
    list->length++;
    return 0;
}

void node_list_remove_node(list_t* list, list_node_t* node)
{
    if (node->next == NULL)
    {
        list->end = node->previous;
    }
    if (node->previous == NULL)
    {
        list->begin = node->next;
    }
    if (node->next != NULL)
    {
        node->next->previous = node->previous;
    }
    if (node->previous != NULL)
    {
        node->previous->next = node->next;
    }
    list->length--;
    free(node);
}

void node_list_free(list_t* list)
{
    list_node_t* node = list->begin;
    while (node != NULL)
    {
        list_node_t* next = node->next;
        free(node);

        #if __DEBUG__
        printf("node at %p freed\n", node);
        #endif

        node = next;
    }
    free(list);

    #if __DEBUG__
    printf("list at %p freed\n", list);
    #endif
}
