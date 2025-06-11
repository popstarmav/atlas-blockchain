int llist_add_node(llist_t *list, void *node, int mode)
{
    llist_t *new_node, *current;
    
    if (!list)
        return (-1);
        
    /* If list is empty, add as first node */
    if (!list->node)
    {
        list->node = node;
        return (0);
    }
    
    /* Create new node */
    new_node = malloc(sizeof(llist_t));
    if (!new_node)
        return (-1);
        
    new_node->node = node;
    new_node->next = NULL;
    new_node->prev = NULL;
    
    if (mode == ADD_NODE_FRONT)
    {
        /* Add at front by shifting existing data */
        new_node->node = list->node;
        new_node->next = list->next;
        new_node->prev = list;
        
        if (list->next)
            list->next->prev = new_node;
            
        list->node = node;
        list->next = new_node;
    }
    else /* ADD_NODE_REAR */
    {
        /* Add at rear - find the last node */
        current = list;
        while (current->next)
            current = current->next;
        
        /* Add new node at the end */
        current->next = new_node;
        new_node->prev = current;
    }
    
    return (0);
}

