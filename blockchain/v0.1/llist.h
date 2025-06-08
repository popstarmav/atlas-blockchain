#ifndef LLIST_H
#define LLIST_H

/**
 * struct llist_s - Linked list node structure
 * @node: Pointer to the stored data
 * @next: Pointer to the next node in the list
 * @prev: Pointer to the previous node in the list
 */
typedef struct llist_s
{
	void *node;
	struct llist_s *next;
	struct llist_s *prev;
} llist_t;

/* Function prototypes */
llist_t *llist_create(int mt_support);
int llist_destroy(llist_t *list, int free_nodes, void (*free_node)(void *));
int llist_add_node(llist_t *list, void *node, int mode);
void *llist_get_head(llist_t *list);
void *llist_get_tail(llist_t *list);
int llist_size(llist_t *list);
int llist_for_each(llist_t *list, int (*func)(void *, unsigned int, void *), void *arg);

/* Type definitions for callbacks */
typedef int (*node_func_t)(void *, unsigned int, void *);
typedef void (*node_dtor_t)(void *);

/* Constants */
#define ADD_NODE_FRONT 0
#define ADD_NODE_REAR 1
#define MT_SUPPORT_FALSE 0
#define MT_SUPPORT_TRUE 1

#endif /* LLIST_H */
