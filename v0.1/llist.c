#include "llist.h"
#include <stdlib.h>

/**
 * llist_create - Creates a new linked list
 * @mt_support: Multi-threading support flag
 * Return: Pointer to the new list, or NULL on failure
 */
llist_t *llist_create(int mt_support)
{
    llist_t *list;
    
    (void)mt_support; /* Unused parameter */
    
    list = malloc(sizeof(llist_t));
    if (!list)
        return (NULL);
        
    list->node = NULL;
    list->next = NULL;
    list->prev = NULL;
    
    return (list);
}

/**
 * llist_add_node - Adds a node to the list
 * @list: Pointer to the list
 * @node: Node to add
 * @add_mode: Where to add the node
 * Return: 0 on success, -1 on failure
 */
int llist_add_node(llist_t *list, void *node, int add_mode)
{
    llist_t *new_node;
    
    if (!list || !node)
        return (-1);
        
    if (!list->node)
    {
        list->node = node;
        return (0);
    }
    
    new_node = malloc(sizeof(llist_t));
    if (!new_node)
        return (-1);
        
    new_node->node = node;
    new_node->next = NULL;
    new_node->prev = list;
    
    if (add_mode == ADD_NODE_REAR)
    {
        while (list->next)
            list = list->next;
        list->next = new_node;
    }
    
    return (0);
}

/**
 * llist_get_head - Gets the head node of the list
 * @list: Pointer to the list
 * Return: Pointer to the head node
 */
void *llist_get_head(llist_t *list)
{
    if (!list)
        return (NULL);
    return (list->node);
}

/**
 * llist_destroy - Destroys a linked list
 * @list: List to destroy
 * @free_nodes: Whether to free the nodes
 * @free_func: Function to free nodes with
 */
void llist_destroy(llist_t *list, int free_nodes, void (*free_func)(void *))
{
    llist_t *current, *next;
    
    if (!list)
        return;
        
    current = list;
    while (current)
    {
        next = current->next;
        if (free_nodes && current->node)
        {
            if (free_func)
                free_func(current->node);
            else
                free(current->node);
        }
        free(current);
        current = next;
    }
}
