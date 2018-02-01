#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define BLACK true
#define RED false

typedef struct node{
    bool color;
    int datum;
    struct node *parent;
    struct node *left;
    struct node *right;
} Node;

void print (Node *tree);
void clear (Node **tree);
int insert (Node **tree, int datum);
void insertBST (Node **tree, Node *node);
bool isLeaf (Node *node);
void insertRepairTree (Node *node);
Node * getUncle (Node *node);
Node * getSibling (Node *node);
Node * getGrandparent (Node *node);
void rotateRight (Node *node);
void rotateLeft (Node *node);
int delete (Node *root, int datum, Node **theTree);
void deleteCase1 (Node *node);
void deleteCase2 (Node *node);
void deleteCase3 (Node *node);
void deleteCase4 (Node *node);
void deleteCase5 (Node *node);
void deleteCase6 (Node *node);
Node * successor (Node *node);
void replace (Node *original, Node *replacement);

int main (void)
{
    Node *tree = NULL;

    insert(&tree, 41);
	insert(&tree, 38);
	insert(&tree, 31);
	insert(&tree, 12);
	insert(&tree, 19);
	insert(&tree, 8);

	print(tree);
    printf("\n-------\n");

	delete(tree, 8, &tree);

	print(tree);
    printf("\n-------\n");

	delete(tree, 12, &tree);

	print(tree);
    printf("\n-------\n");

	delete(tree, 19, &tree);

	print(tree);
    printf("\n-------\n");

	delete(tree, 31, &tree);

	print(tree);
    printf("\n-------\n");

	delete(tree, 38, &tree);

	print(tree);
    printf("\n-------\n");

	delete(tree, 41, &tree);

    clear(&tree);

	print(tree);
    printf("\n-------\n");

    return 0;
}

void print (Node *tree)
{
    static int depth = 0;
    if(!tree) return;
    ++depth;
    print(tree->right);
    --depth;
    for(int i = 0; i < depth; ++i)
        printf("   ");
    if(isLeaf(tree))
        printf("NIL\n");
    else if(tree->color == RED)
        printf("\x1b[91m%d\x1b[0m\n",tree->datum);
    else
        printf("%d\n",tree->datum);
    ++depth;
    print(tree->left);
    --depth;
    return;
}

void clear (Node **tree)
{
    if(!*tree) return;
    clear(&((*tree)->left));
    clear(&((*tree)->right));
    free(*tree);
    *tree = NULL;
    return;
}

int insert (Node **tree, int datum)
{
    Node *node = malloc(sizeof(Node));

	Node *leftNIL = malloc(sizeof(Node));
    leftNIL->datum = 0;
	leftNIL->parent = node;
	leftNIL->left = NULL;
	leftNIL->right = NULL;
	leftNIL->color = BLACK;

	Node *rightNIL = malloc(sizeof(Node));
	rightNIL->datum = 0;
	rightNIL->parent = node;
	rightNIL->left = NULL;
	rightNIL->right = NULL;
	rightNIL->color = BLACK;
    node->color = RED;
    node->datum = datum;
    node->parent = NULL;
    node->left = leftNIL;
    node->right = rightNIL;

    insertBST(tree, node);
    insertRepairTree(node);

    while(node->parent){
        node = node->parent;
    }
    *tree = node;

    return 0;
}

void insertBST (Node **tree, Node *node)
{
    Node *treeCopy = *tree;
    if (!treeCopy)
	{
        node->color = BLACK;
        *tree = node;
        return;
    }
	/****LEFT****/
    if (node->datum < treeCopy->datum)
	{
        if (isLeaf(treeCopy->left))
		{
            node->parent = treeCopy;
            free(treeCopy->left);
            treeCopy->left = node;
            return;
        }
        insertBST(&(treeCopy->left), node);
        return;
    }
	/****RIGHT****/
    if (node->datum >= treeCopy->datum)
	{
        if (isLeaf(treeCopy->right))
		{
            node->parent = treeCopy;
            free(treeCopy->right);
            treeCopy->right = node;
            return;
        }
        insertBST(&(treeCopy->right), node);
        return;
    }
    return;
}

bool isLeaf (Node *node)
{
	// only works because leaves are NIL
    if (node->left == NULL)
        return true;
    return false;
}

void insertRepairTree (Node *node)
{
    if (node->parent == NULL && node->color == RED)
	{
        node->color = BLACK;
        return;
    }
    if (node->parent && node->parent->parent)
	{
        if (node->parent->color == BLACK)
            return;
        if (getUncle(node)->color == RED)
		{
            node->parent->color = BLACK;
            getUncle(node)->color = BLACK;
            getGrandparent(node)->color = RED;
            insertRepairTree(getGrandparent(node));
            return;
        }
        if (getUncle(node)->color == BLACK)
		{
            if (getGrandparent(node)->left->right == node)
			{
                rotateLeft(node->parent);
                node = node->left;
            }
            else if (getGrandparent(node)->right->left == node)
			{
                rotateRight(node->  parent);
                node = node->right;
            }
            if(node->parent->left == node)
                rotateRight(getGrandparent(node));
            else
                rotateLeft(getGrandparent(node));
            node->parent->color = BLACK;
            getSibling(node)->color = RED;
        }
    }
    return;
}

Node * getUncle (Node *node)
{
    if (node->parent == NULL) return NULL;
    return getSibling(node->parent);
}

Node * getSibling (Node *node)
{
    if (node->parent == NULL) return NULL;
    if (node->parent->left == node)
        return node->parent->right;
    return node->parent->left;
}

Node * getGrandparent (Node *node)
{
    if (node->parent == NULL) return NULL;
    return node->parent->parent;
}

void rotateRight (Node *node)
{
    Node *pivot = node->left;
    node->left = pivot->right;
    node->left->parent = node;
    pivot->right = node;
    if(node->parent)
	{
        pivot->parent = node->parent;
        if (pivot->parent->left == node)
            pivot->parent->left = pivot;
        else
            pivot->parent->right = pivot;
    }
    else
        pivot->parent = NULL;
    node->parent = pivot;
    return;
}

void rotateLeft (Node *node)
{
    Node *pivot = node->right;
    node->right = pivot->left;
    node->right->parent = node;
    pivot->left = node;
    if (node->parent)
	{
        pivot->parent = node->parent;
        if (pivot->parent->right == node)
            pivot->parent->right = pivot;
        else
            pivot->parent->left = pivot;
    }
    else
        pivot->parent = NULL;
    node->parent = pivot;
    return;
}

int delete (Node *root, int datum, Node **theTree)
{
    if (isLeaf(root)) return 0;
    Node *curr = root;
    if (curr->datum == datum)
    {
        if ((isLeaf(curr->left)) || (isLeaf(curr->right))) // one child
        {
            Node *child = (!isLeaf(curr->left)) ? curr->left : curr->right;
            replace(curr, child);
            if (curr->color == BLACK)
            {
                if (child->color == RED)
                    child->color = BLACK;
                else
                    deleteCase1(child);
            }
            Node *tmp = child;
            while (tmp->parent)
                tmp = tmp->parent;
            *theTree = tmp;
            free(root);
            return 0;
        }
        else // two children
        {
            Node *leftest = successor(curr);
            curr->datum = leftest->datum;
            return delete(leftest, curr->datum, theTree);
        }
    }
    if (curr->datum > datum)
        return delete(curr->left, datum, theTree);
    return delete(curr->right, datum, theTree);
}

void deleteCase1 (Node *n)
{
    if (n->parent != NULL)
        deleteCase2(n);
}

void deleteCase2 (Node *n)
{
    Node *s = getSibling(n);
    Node *p = n->parent;

    if (s->color == RED)
    {
        p->color = RED;
        s->color = BLACK;
        if (n == p->left)
            rotateLeft(p);
        else
            rotateRight(p);
    }
    deleteCase3(n);
}

void deleteCase3 (Node *n)
{
    Node *s = getSibling(n);
    Node *p = n->parent;

    if ((p->color == BLACK)
            && (s->color == BLACK)
            && (s->left->color == BLACK)
            && (s->right->color == BLACK))
    {
        s->color = RED;
        if (p->parent != NULL) { deleteCase2(p); }
    }
    else
        deleteCase4(n);
}

void deleteCase4 (Node *n)
{
    Node *s = getSibling(n);
    Node *p = n->parent;

    if ((p->color == RED)
            && (s->color == BLACK)
            && (s->left->color == BLACK)
            && (s->right->color == BLACK))
    {
        s->color = RED;
        p->color = BLACK;
    }
    else
        deleteCase5(n);
}

void deleteCase5 (Node *n)
{
    Node *s = getSibling(n);
    Node *p = n->parent;

    if (s->color == BLACK)
    {
        if ((n == p->left)
                && (s->right->color == BLACK)
                && (s->left->color == RED))
        {
            s->color = RED;
            s->left->color = BLACK;
            rotateRight(s);
        }
        else if ((n == p->right)
                && (s->left->color==BLACK)
                && (s->right->color==RED))
        {
            s->color = RED;
            s->right->color = BLACK;
            rotateLeft(s);
        }
    }
    deleteCase6(n);
}

void deleteCase6 (Node *n)
{
    Node *s = getSibling(n);
    Node *p = n->parent;

    s->color = p->color;
    p->color = BLACK;

    if (n == p->left)
    {
        s->right->color = BLACK;
        rotateLeft(p);
    }
    else
    {
        s->left->color = BLACK;
        rotateRight(p);
    }
}

Node * successor(Node *node)
{
    Node *curr = node->right;
    while (isLeaf(curr->left) == false)
        curr = curr->left;
    return curr;
}

void replace (Node *original, Node *replacement)
{
    Node *p = original->parent;
    if (p)
    {
        if (p->left == original)
            p->left = replacement;
        else
            p->right = replacement;
    }
    replacement->parent = p;
}
