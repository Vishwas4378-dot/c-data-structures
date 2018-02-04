#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define BLACK true
#define RED false

typedef struct node
{
    int datum;
    struct node *parent;
    struct node *left;
    struct node *right;
} Node;

typedef struct scapegoatTree
{
	int size;
	Node *root;
	int n; // number of nodes in the tree
	int maxNodeCount;
} ScapegoatTree;

void print (Node *tree);
void clear (Node **root);
bool insert (ScapegoatTree **sgTree, int datum);
int insertBST (ScapegoatTree **sgTree, Node *node);
void rebuildTree (ScapegoatTree *sgTree, Node *node);
int storeInArray (Node *ptr, Node *arr[], int i);
int size (Node *node);
Node * buildBalancedFromArray(Node **a, int i, int n);
int delete (Node **tree, int value, ScapegoatTree **sgTree);
Node * min (Node *node);
int getHeight (Node *node);

static int log32 (int n)
{
    double const log23 = 2.4663034623764317;
    return (int)ceil(log23 * log(n));
}

int main (void)
{
    ScapegoatTree *sgTree = NULL;

    insert(&sgTree, 7);
	insert(&sgTree, 3);
	insert(&sgTree, 6);
	insert(&sgTree, 1);
	insert(&sgTree, 0);
	insert(&sgTree, 8);
	insert(&sgTree, 9);
	insert(&sgTree, 4);
	insert(&sgTree, 5);
	insert(&sgTree, 2);
	insert(&sgTree, 1);
	insert(&sgTree, 0);
	insert(&sgTree, 8);
	insert(&sgTree, 9);
	insert(&sgTree, 4);
	insert(&sgTree, 5);
	insert(&sgTree, 2);
	insert(&sgTree, 40);
	insert(&sgTree, 41);
	insert(&sgTree, 41);
	insert(&sgTree, 41);
	insert(&sgTree, 41);
	insert(&sgTree, 41);
	insert(&sgTree, 41);
	insert(&sgTree, 41);
	insert(&sgTree, 41);

	print(sgTree->root);
    printf("-------\n");

	delete(&(sgTree->root), 4, &sgTree);
	print(sgTree->root);
    printf("-------\n");

	delete(&(sgTree->root), 3, &sgTree);

	print(sgTree->root);
    printf("-------\n");

    clear(&(sgTree->root));

	print(sgTree->root);
    printf("-------\n");

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
    printf("%d\n",tree->datum);
    ++depth;
    print(tree->left);
    --depth;
    return;
}

void clear (Node **root)
{
    if(!*root) return;
    clear(&((*root)->left));
    clear(&((*root)->right));
    free(*root);
    *root = NULL;
    return;
}

bool insert (ScapegoatTree **sgTree, int datum)
{
    Node *node = malloc(sizeof(Node));
    node->datum = datum;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    int h = insertBST(sgTree, node);

	if (h > log32((*sgTree)->n))
	{
		// find the scapegoat
		Node *p = node->parent;
		while (3 * size(p) > 2 * size(p->parent))
			p = p->parent;

		// now rebuild under that scapegoat
		rebuildTree(*sgTree, p->parent);
	}

    while(node->parent){
        node = node->parent;
    }
    (*sgTree)->root = node;
	(*sgTree)->size = size((*sgTree)->root);
	if ((*sgTree)->size > (*sgTree)->maxNodeCount)
		(*sgTree)->maxNodeCount = (*sgTree)->size;

    return h >= 0;
}

int insertBST (ScapegoatTree **sgTree, Node *node)
{
	// Case 1: Empty tree
	if (*sgTree == NULL)
	{
		*sgTree = malloc(sizeof(ScapegoatTree));
		(*sgTree)->root = node;
		(*sgTree)->n++;
		return 0;
	}
	// Case 2: Find where to insert, done non-recursively
	bool done = false;
	int d = 0;
	while (!done)
	{
		if (node->datum < (*sgTree)->root->datum)
		{
			if ((*sgTree)->root->left == NULL)
			{
				node->parent = (*sgTree)->root;
				(*sgTree)->root->left = node;
				done = true;
			}
			else
				(*sgTree)->root = (*sgTree)->root->left;
		}
		else if (node->datum >= (*sgTree)->root->datum)
		{
			if ((*sgTree)->root->right == NULL)
			{
				node->parent = (*sgTree)->root;
				(*sgTree)->root->right = node;
				done = true;
			}
			else
				(*sgTree)->root = (*sgTree)->root->right;
		}
		else
			return -1;
		++d;
	}
	(*sgTree)->n++;
	return d;
}

void rebuildTree (ScapegoatTree *sgTree, Node *node)
{
	int n = size(node);
	Node *p = node->parent;
	Node *a_1[n];
	Node **a = a_1;
	storeInArray(node, a, 0);
	if (p == NULL)
	{
		sgTree->root = buildBalancedFromArray(a, 0, n);
		sgTree->root->parent = NULL;
	}
	else if (p->right == node)
	{
		p->right = buildBalancedFromArray(a, 0, n);
		p->right->parent = p;
	}
	else
	{
		p->right = buildBalancedFromArray(a, 0, n);
		p->left->parent = p;
	}
}

int size (Node *node)
{
	if (node == NULL)
		return 0;
	return 1 + size(node->left) + size(node->right);
}

int storeInArray (Node *ptr, Node *arr[], int i)
{
	if (ptr == NULL) return i;
	i = storeInArray(ptr->left, arr, i);
	arr[i++] = ptr;
	return storeInArray(ptr->right, arr, i);
}

Node * buildBalancedFromArray(Node **a, int i, int n)
{
	if (n == 0) return NULL;

	int m = n / 2;

	// a[m] becomes root of new subtree
	a[i+m]->left = buildBalancedFromArray(a, i, m);

	// a[0] to a[m-1] are stored in left subtree
	if (a[i+m]->left != NULL)
		a[i+m]->left->parent = a[i+m];

	// a[m+1] to a[n-1] are stored in right subtree
	a[i+m]->right = buildBalancedFromArray(a, i+m+1, n-m-1);
	if (a[i+m]->right != NULL)
		a[i+m]->right->parent = a[i+m];

	return a[i+m];
}

int delete (Node **root, int datum, ScapegoatTree **sgTree)
{
	Node *tmp = *root;
	if(tmp == NULL) return -1;
	if(datum < tmp->datum)
	{
		int depth = delete(&(tmp->left), datum, sgTree);
		return depth == -1 ? -1 : 1 + depth;
	}
	if(datum > tmp->datum)
	{
		int depth = delete(&(tmp->right), datum, sgTree);
		return depth == -1 ? -1 : 1 + depth;
	}
	if(tmp->left && tmp->right)
	{
		Node *next = min(tmp->right);
		tmp->datum = next->datum;
		delete(&(tmp->right), next->datum, sgTree);
		return 0;
	}
	if(tmp->left == NULL)
		*root = tmp->right;
	else //if(tmp->right == NULL)
		*root = tmp->left;
	free(tmp);

	(*sgTree)->size = size((*sgTree)->root);

	if ((*sgTree)->size < (*sgTree)->maxNodeCount * (3.0/2))
	{
		rebuildTree(*sgTree, (*sgTree)->root);
	}

	return 0;
}

Node *min(Node *root)
{
  if(root->left == NULL)
    return root;
  return min(root->left);
}

int getHeight (Node *node)
{
	if (node->parent == NULL) return 0;
	return 1 + getHeight(node->parent);
}
