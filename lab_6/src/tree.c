#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

#define NUM_OF_SPACES    5

#define TREE_HEIGHT(p)  ((p) ? p->fact : 0)
#define TREE_BFACTOR(p) (TREE_HEIGHT(p->right) - TREE_HEIGHT(p->left))


static inline void tree_fixheight(tnode_t *p)
{
    int hl = TREE_HEIGHT(p->left);
    int hr = TREE_HEIGHT(p->right);

    p->fact = (hl > hr ? hl : hr) + 1;
}

static inline tnode_t *tree_rotate_left(tnode_t *q)
{
    tnode_t *p = q->right;
    
    q->right = p->left;
    p->left = q;

    tree_fixheight(q);
    tree_fixheight(p);

    return p;
}

static inline tnode_t *tree_rotate_right(tnode_t *p)
{
    tnode_t *q = p->left;
    
    p->left = q->right;
    q->right = p;

    tree_fixheight(p);
    tree_fixheight(q);

    return q;
}

tnode_t *balance_node(tnode_t *node)
{
    tree_fixheight(node);

    if (TREE_BFACTOR(node) == 2)
    {
        if (TREE_BFACTOR(node->right) < 0)
            node->right = tree_rotate_right(node->right);
        return tree_rotate_left(node);
    }

    if (TREE_BFACTOR(node) == -2)
    {
        if (TREE_BFACTOR(node->left) > 0)
            node->left = tree_rotate_left(node->left);
        return tree_rotate_right(node);
    }



    return node;
}

void tree_print(tnode_t *tree)
{
    if (tree)
    {
        printf("%d  ", tree->data);
        tree_print(tree->left);
        tree_print(tree->right);
    }
}

tnode_t *tree_insert(tnode_t *tree, int val)
{
    if (tree == NULL)
        return init_node(val);
    
    if (val < tree->data)
        tree->left = tree_insert(tree->left, val);
    else
        tree->right = tree_insert(tree->right, val);
    
    return tree;
}

tnode_t *tree_insert_b(tnode_t *tree, int val)
{
    if (tree == NULL)
        return init_node(val);
    
    if (val < tree->data)
        tree->left = tree_insert_b(tree->left, val);
    else
        tree->right = tree_insert_b(tree->right, val);
    
    return balance_node(tree);
}

tnode_t *init_node(int val)
{
    tnode_t *node = malloc(sizeof(tnode_t));

    if (node == NULL)
        return NULL;
    
    node->fact = 1;
    node->data = val;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

void tree_clean(tnode_t *tree)
{
    if (tree)
    {
        tree_clean(tree->left);
        tree_clean(tree->right);
        free(tree);
    }
}

tnode_t *tree_find(tnode_t *tree, int key)
{
    if (tree == NULL)
        return NULL;

    if (tree->data == key)
        return tree;

    if (tree->data < key)
        return tree_find(tree->right, key);

    if (tree->data > key)
        return tree_find(tree->left, key);
}


static void draw_tree_hor2(tnode_t *tree, int depth, char *path, int right)
{
    if (tree == NULL)
        return;

    depth++;

    draw_tree_hor2(tree->right, depth, path, 1);

    if (depth > 1)
    {
        path[depth - 2] = 0;

        if (right)
            path[depth - 2] = 1;
    }

    if (tree->left)
        path[depth - 1] = 1;

    printf("\n");

    for (int i = 0; i < depth - 1; i++)
    {
        if (i == depth - 2)
            printf("+");
        else if (path[i])
            printf("|");
        else
            printf(" ");

        for (int j = 1; j < NUM_OF_SPACES; j++)
            if (i < depth - 2)
                printf(" ");
            else
                printf("-");
    }

    printf("[%d]\n", tree->data);

    for (int i = 0; i < depth; i++)
    {
        if (path[i])
            printf("|");
        else
            printf(" ");

        for (int j = 1; j < NUM_OF_SPACES; j++)
            printf(" ");
    }

    draw_tree_hor2(tree->left, depth, path, 0);
}

void draw_tree_hor(tnode_t *tree)
{
    char *path = malloc(sizeof(char) * 2048);

    if (path == NULL)
        return;

    draw_tree_hor2(tree, 0, path, 0);
    free(path);
}

int tree_depth(tnode_t *tree)
{
    if (tree == NULL)
        return 0;

    int left = tree_depth(tree->left);
    int right = tree_depth(tree->right);

    if (left > right)
        return left + 1;
    else
        return right + 1;
}

void tree_cmprs(tnode_t *tree, int *vert, int *cmps, int depth)
{
    if (tree == NULL)
        return;
    
    (*vert)++;
    (*cmps) += depth;

    tree_cmprs(tree->left, vert, cmps, depth + 1);
    tree_cmprs(tree->right, vert, cmps, depth + 1);
}

int tree_find_cmps(tnode_t *tree, int key)
{
    if (tree == NULL)
        return -1;

    if (tree->data == key)
        return 1;

    int cmps = 0;

    if (tree->data < key)
    {
        cmps = tree_find_cmps(tree->right, key);
        return cmps > 0 ? cmps + 1 : cmps;
    }

    if (tree->data > key)
    {
        cmps = tree_find_cmps(tree->left, key);
        return cmps > 0 ? cmps + 1 : cmps;
    }
}


void display(tnode_t *tree, int level)
{
	int i;
	if (tree != NULL)
	{
		display(tree->right, level + 1);
		printf("\n");
	
    	for (i = 0; i < level; i++)
			printf("     ");
		printf("%d", tree->data);
    
		display(tree->left, level + 1);
	}
}
