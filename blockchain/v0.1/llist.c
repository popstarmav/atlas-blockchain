#include "llist.h"
#include <stdlib.h>

llist_t *llist_create(int mt_support)
{
    llist_t *list;
    
    (void)mt_support;
    
    list = malloc(sizeof(llist_t));
    if (!list)
        return (NULL);
        
    list->node = NULL;
    list->next = NULL;
    list->prev = NULL;
    
    return (list);
}

int llist_destroy(llist_t *list, int free_nodes, void (*free_node)(void *))
{
    llist_t *current, *next;
    
    if (!list)
        return (-1);
        
    current = list;
    while (current)
    {
        next = current->next;
        if (free_nodes && current->node)
        {
            if (free_node)
                free_node(current->node);
            else
                free(current->node);
        }
        free(current);
        current = next;
    }
    
    return (0);
}

int llist_add_node(llist_t *list, void *node, int mode)
{
    llist_t *new_element;
    llist_t *current;
    
    if (!list || !node)
        return (-1);
    
    /* If this is the first node being added */
    if (list->node == NULL)
    {
        list->node = node;
        return (0);
    }
    
    /* Allocate new element */
    new_element = malloc(sizeof(llist_t));
    if (!new_element)
        return (-1);
    
    new_element->node = node;
    new_element->next = NULL;
    new_element->prev = NULL;
    
    if (mode == ADD_NODE_REAR)
    {
        /* Find the last element */
        current = list;
        while (current->next != NULL)
            current = current->next;
        
        /* Attach new element at the end */
        current->next = new_element;
        new_element->prev = current;
    }
    else /* ADD_NODE_FRONT */
    {
        /* Save the current head's data */
        new_element->node = list->node;
        new_element->next = list->next;
        new_element->prev = list;
        
        /* Update existing connections */
        if (list->next)
            list->next->prev = new_element;
        
        /* Make new node the head */
        list->node = node;
        list->next = new_element;
    }
    
    return (0);
}

void *llist_get_head(llist_t *list)
{
    if (!list)
        return (NULL);
    return (list->node);
}

void *llist_get_tail(llist_t *list)
{
    llist_t *current;
    
    if (!list || !list->node)
        return (NULL);
        
    current = list;
    while (current->next)
        current = current->next;
    
    return (current->node);
}

int llist_size(llist_t *list)
{
    llist_t *current;
    int count = 0;
    
    if (!list)
        return (0);
        
    current = list;
    while (current != NULL && current->node != NULL)
    {
        count++;
        current = current->next;
        
        /* Prevent infinite loops */
        if (count > 1000)
            return (-1);
    }
    
    return (count);
}

int llist_for_each(llist_t *list, int (*func)(void *, unsigned int, void *), void *arg)
{
    llist_t *current;
    unsigned int index = 0;
    
    if (!list || !func)
        return (-1);
        
    current = list;
    while (current != NULL && current->node != NULL)
    {
        if (func(current->node, index, arg) != 0)
            return (-1);
        
        current = current->next;
        index++;
        
        /* Prevent infinite loops */
        if (index > 1000)
            return (-1);
    }
    
    return (0);
}

