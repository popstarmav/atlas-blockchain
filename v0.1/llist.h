#ifndef LLIST_H
#define LLIST_H

#define MT_SUPPORT_FALSE 0
#define ADD_NODE_REAR 1

/**
 * struct llist_s - Linked list structure
 * @node: Pointer to a node of the list
 * @next: Pointer to the next element of the list
 */
typedef struct llist_s
{
    void            *node;
    struct llist_s  *next;
    struct llist_s  *prev;
} llist_t;

/* Function prototypes */
llist_t *llist_create(int mt_support);
int llist_add_node(llist_t *list, void *node, int add_mode);
void *llist_get_head(llist_t *list);
void llist_destroy(llist_t *list, int free_nodes, void (*free_func)(void *));

#endif /* LLIST_H */
