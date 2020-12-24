#ifndef __TREE_H__
#define __TREE_H__

struct tnode_s
{
    int data;
    struct tnode_s *left;
    struct tnode_s *right;
    int fact;
};

typedef struct tnode_s tnode_t;

void tree_print(tnode_t *tree); // in depth

tnode_t *tree_insert(tnode_t *tree, int val);
tnode_t *tree_insert_b(tnode_t *tree, int val);


tnode_t *init_node(int val);
void tree_clean(tnode_t *tree);

tnode_t *balance_node(tnode_t *node);

tnode_t *tree_find(tnode_t *tree, int key);

void draw_tree_hor(tnode_t *tree);
int tree_depth(tnode_t *tree);
void tree_cmprs(tnode_t *tree, int *vert, int *cmps, int depth);
int tree_find_cmps(tnode_t *tree, int key);
void display(tnode_t *tree, int level);

#endif // __TREE_H__
