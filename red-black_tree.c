// AVL TREE: rotate right/left, successor, either store the balance factor in the struct or get the height

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define BLACK true
#define RED false

typedef struct node {
    bool color;
    int datum;
    struct node *parent;
    struct node *left;
    struct node *right;
} Node;

void print (Node *tree);
void clear (Node **tree);
void insert (Node **tree, int datum);
int insertBST (Node **tree, Node *new);
void insertRepairTree (Node **tree, Node *new);
void insertCase3 (Node **tree, Node *new);
void insertCase4 (Node **tree, Node *node);
void case4_2 (Node **tree, Node *node);
void rotateLeft (Node *node, Node **tree);
void rotateRight (Node *node, Node **tree);
Node * getGrandparent (Node *node);
Node * getSibling (Node *node);
Node * getUncle (Node *node);
short childType (Node *node);
int delete (Node **tree, int value);
Node * successor (Node *node);

Node tmp = {BLACK, 0, NULL, NULL, NULL}; // datum should really be NULL but there are annoying warnings otherwise
Node *LEAF = &tmp;

int main (void) 
{
    Node *tree = NULL;

    insert(&tree, 6);
    insert(&tree, 4);
    insert(&tree, 9);
    insert(&tree, 3);
    insert(&tree, 4);
    insert(&tree, 1);
    insert(&tree, 10);
    insert(&tree, 12);
    insert(&tree, 13);
    insert(&tree, 12);
    insert(&tree, 9);
    insert(&tree, 5);
    insert(&tree, 8);
    insert(&tree, 7);
    insert(&tree, 9);
    insert(&tree, 5);

    print(tree);
    printf("\n--------\n");

    delete(&tree, 1);

    print(tree);
    printf("\n-------\n");

    delete(&tree, 8);

    print(tree);
    printf("\n-------\n");

	clear(&tree);

    return 0;
}

void print (Node *tree)
{
    static int depth = 0;
    if (tree == NULL)
    {
        return;
    }
    ++depth;
    print(tree->right);
    --depth;
    for (int i = 0; i < depth; ++i)
    {
        printf("  ");
    }
    if (tree->color == RED)
    {
        printf("\x1b[91m%d\x1b[0m\n", tree->datum); // set the print color to red, print, and reset the print color
    }
    else 
    {
        if (tree == LEAF) 
        {
            printf("NIL\n");
        } 
        else 
        {
            printf("%d\n", tree->datum);
        }
    }
    ++depth;
    print(tree->left);
    --depth;
}

void clear (Node **tree)
{
	if(*tree == LEAF) return;

	clear(&((*tree)->left));
	clear(&((*tree)->right));
	free(*tree);
	*tree = NULL;
}

void insert (Node **tree, int datum)
{
    Node *new = malloc(sizeof(Node));
    new->color = RED;
    new->datum = datum;
    new->parent = NULL;
    new->left = LEAF;
    new->right = LEAF;

    insertBST(tree, new);

    insertRepairTree(tree, new);
}

int insertBST (Node **tree, Node *new)
{
	if (*tree == NULL)
    {
        (*tree) = new;
        (*tree)->color = BLACK; // root is always black
        return 0;
    }

    if (new->datum < (*tree)->datum)
    {
        if ((*tree)->left != LEAF)
        {
            return 1 + insertBST(&((*tree)->left), new);
        }
        else
        {
            new->parent = (*tree);
            (*tree)->left = new;
        }
    }
    else
    {
		if ((*tree)->right != LEAF)
        {
            return 1 + insertBST(&((*tree)->right), new);
        }
        else
        {
            new->parent = (*tree);
            (*tree)->right = new;
        }
    }

    return 0;
}

void insertRepairTree (Node **tree, Node *new)
{
    if (new->parent == NULL)
    {
        new->color = BLACK;
        return;
    }
    if (new->parent->color == BLACK)
    {
        return;
    }
    if (getUncle(new)->color == RED)
    {
        insertCase3(tree, new);
        return;
    }
    else if (getUncle(new)->color == BLACK)
    {
        insertCase4(tree, new);
    }
}

void insertCase3 (Node **tree, Node *node)
{
    Node *g = getGrandparent(node);
    g->color = RED;
    node->parent->color = BLACK;
    getUncle(node)->color = BLACK;
    insertRepairTree(tree, g);
}

void insertCase4 (Node **tree, Node *node)
{
    Node *p = node->parent;
    Node *g = getGrandparent(node);

    if (node == g->left->right)
    {
        rotateLeft(p, tree);
        node = node->left;
    }
    else if (node == g->right->left)
    {
        rotateRight(p, tree);
        node = node->right;
    }

    case4_2(tree, node);
}

void case4_2 (Node **tree, Node *node)
{
    Node *p = node->parent;
    Node *g = getGrandparent(node);

    if (node == p->left)
    {
        rotateRight(g, tree);
    }
    else
        rotateLeft(g, tree);
    p->color = BLACK;
    g->color = RED;
}

void rotateLeft (Node *node, Node **tree)
{
    Node *right = node->right;
    node->right = right->left;
    if (node->right != NULL)
        node->right->parent = node;
    right->parent = node->parent;
    if (node->parent == NULL)
        *tree = right;
    else if (node == node->parent->left)
        node->parent->left = right;
    else
        node->parent->right = right;
    right->left = node;
    node->parent = right;
}

void rotateRight (Node *node, Node **tree)
{
    Node *left = node->left;
    node->left = left->right;
    if (node->left != NULL)
        node->left->parent = node;
    left->parent = node->parent;
    if (node->parent == NULL)
        *tree = left;
    else if (node == node->parent->left)
        node->parent->left = left;
    else
        node->parent->right = left;
    left->right = node;
    node->parent = left;
}

Node * getGrandparent (Node *node)
{
    if (node->parent)
        return node->parent->parent;
    return NULL;
}

Node * getSibling (Node *node)
{
    Node *p = node->parent;
    if (!p)
    {
        return NULL;
    }
    if (p->left == node)
    {
        return p->right;
    }
    return p->left;
}

Node * getUncle (Node *node)
{
    if (!node->parent)
        return NULL;
    return getSibling(node->parent);
}

short childType (Node *node) // -1 is left, 0 is root, 1 is right
{
    if (node->parent == NULL) return 0;
    if ((node->parent)->left == node) return -1;
    return 1;
}

int delete (Node **tree, int value)
{
    Node *tmp = *tree;
    if (tmp == LEAF) return 0;
    if (value < tmp->datum)
        return delete(&(tmp->left), value);
    if (value > tmp->datum)
        return delete(&(tmp->right), value);
    if ((tmp->left == LEAF) != (tmp->right == LEAF)) // case 1: one and only one LEAF child
    {
        Node *child = tmp->left == LEAF ? tmp->right : tmp->left;
        tmp->datum = child->datum;
        tmp->parent = child->parent;
        tmp->right = LEAF;
        tmp->left = LEAF;
        if (tmp->color == BLACK)
        {
            if (child->color == RED)
                child->color = BLACK;
            //else
                // insert delete case 1 here
        }
        free(child);
    }
    if (tmp->left == LEAF && tmp->right == LEAF) // case 2: two LEAF children
    {
		Node *parent = tmp->parent;
        parent->left = LEAF;
        parent->right = LEAF;
        return 0;
    }
    if (tmp->left == NULL)
        *tree = tmp->right;
    else
        *tree = tmp->left;
    free(tmp);
    return 0;
}

Node * successor (Node *node)
{
    Node *cur = node->right;
    while (cur->left != LEAF)
        cur = cur->left;
    return cur;
}
