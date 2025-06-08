#include "llist.h"
#include <stdlib.h>

/**
 * llist_create - Creates a new linked list
 * @mt_support: Multi-threading support flag (ignored for now)
 *
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
 * llist_destroy - Destroys a linked list
 * @list: Pointer to the list
 * @free_nodes: Whether to free the nodes
 * @free_node: Function to free node data
 *
 * Return: 0 on success, -1 on failure
 */
int llist_destroy(llist_t *list, int free_nodes, void (*free_node)(void *))
{
	llist_t *current, *next;

	if (!list)
		return (-1);

	current = list;
	while (current)
	{
		next = current->next;
		if (free_nodes && current->node && free_node)
			free_node(current->node);
		else if (free_nodes && current->node)
			free(current->node);
		free(current);
		current = next;
	}

	return (0);
}

/**
 * llist_add_node - Adds a node to the list
 * @list: Pointer to the list
 * @node: Data to add
 * @mode: Where to add (front or rear)
 *
 * Return: 0 on success, -1 on failure
 */
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
		/* Add at front */
		new_node->next = list;
		list->prev = new_node;
		/* Note: caller should update their list pointer */
	}
	else
	{
		/* Add at rear */
		current = list;
		while (current->next)
			current = current->next;
		current->next = new_node;
		new_node->prev = current;
	}

	return (0);
}

/**
 * llist_get_head - Gets the first node's data
 * @list: Pointer to the list
 *
 * Return: Pointer to the first node's data, or NULL
 */
void *llist_get_head(llist_t *list)
{
	if (!list)
		return (NULL);

	return (list->node);
}

/**
 * llist_get_tail - Gets the last node's data
 * @list: Pointer to the list
 *
 * Return: Pointer to the last node's data, or NULL
 */
void *llist_get_tail(llist_t *list)
{
	llist_t *current;

	if (!list)
		return (NULL);

	current = list;
	while (current->next)
		current = current->next;

	return (current->node);
}

/**
 * llist_size - Gets the size of the list
 * @list: Pointer to the list
 *
 * Return: Number of nodes in the list
 */
int llist_size(llist_t *list)
{
	llist_t *current;
	int count = 0;

	if (!list)
		return (0);

	current = list;
	while (current && current->node)
	{
		count++;
		current = current->next;
	}

	return (count);
}

/**
 * llist_for_each - Applies a function to each node
 * @list: Pointer to the list
 * @func: Function to apply
 * @arg: Additional argument for the function
 *
 * Return: 0 on success, -1 on failure
 */
int llist_for_each(llist_t *list, int (*func)(void *, unsigned int, void *), void *arg)
{
	llist_t *current;
	unsigned int index = 0;

	if (!list || !func)
		return (-1);

	current = list;
	while (current && current->node)
	{
		if (func(current->node, index, arg) != 0)
			return (-1);
		current = current->next;
		index++;
	}

	return (0);
}
