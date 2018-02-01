#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

typedef struct node
{
	int datum;
	int balF;
	struct node* left;
	struct node* right;
} Node;

void print (Node* root);
void clear (Node** root);
int height (Node* node);
int getDiff (Node* node);
Node * insert (Node** root, int datum);
Node * RRrotation (Node* root);
Node * RLrotation (Node* root);
Node * LLrotation (Node* root);
Node * LRrotation (Node* root);
Node * balance (Node* node);
int delete (Node **root, char datum);
Node * min (Node *root);

int main(void)
{
	Node* tree = NULL;

	insert(&tree, 30);
	insert(&tree, 26);
	insert(&tree, 21);
	insert(&tree, 20);
	insert(&tree, 11);
	insert(&tree, 9);
	insert(&tree, 4);
	insert(&tree, 3);
	insert(&tree, 2);
	insert(&tree, 5);
	insert(&tree, 15);

	print(tree);
	printf("\n-------\n");

	delete(&tree, 9);

	print(tree);
	printf("\n-------\n");

	delete(&tree, 5);

	print(tree);
	printf("\n-------\n");

	delete(&tree, 4);

	print(tree);
	printf("\n-------\n");

	delete(&tree, 26);

	print(tree);
	printf("\n-------\n");

	delete(&tree, 21);

	print(tree);
	printf("\n-------\n");

	delete(&tree, 11);

	print(tree);
	printf("\n-------\n");

	delete(&tree, 3);

	print(tree);
	printf("\n-------\n");

	delete(&tree, 30);

	print(tree);
	printf("\n-------\n");

	delete(&tree, 15);

	print(tree);
	printf("\n-------\n");

	delete(&tree, 2);

	print(tree);
	printf("\n-------\n");

	delete(&tree, 20);

	print(tree);
	printf("\n-------\n");

	clear(&tree);

	return 0;
}

int height(Node* node)
{
	int h = 0;
	if (node != NULL)
	{
		int leftH = height(node->left);
		int rightH = height(node->right);
		int max = (leftH > rightH) ? leftH : rightH;
		h = 1 + max;
	}
	return h;
}

int getDiff(Node* node)
{
	int leftH = height(node->left);
	int rightH = height(node->right);
	int balFactor = leftH - rightH;
	return balFactor;
}

// if balance factor == 1, 0, -1, then no rotation necessary
// 4 cases:
// 1. RR if bfr is -1
// 2. RL if bfr is 1
// 3. LL if bfl is 1
// 4. LR if bfl is -1

Node * RRrotation (Node* root)
{
	Node* node;
	node = root->right;
	root->right = node->left;
	node->left = root;
	return node;
}

Node * RLrotation (Node* root)
{
	Node* node;
	node = root->right;
	root->right = LLrotation(node);
	return RRrotation(root);
}

Node * LLrotation (Node* root)
{
	Node* node;
	node = root->left;
        root->left = node->right;
	node->right = root;
	return node;
}

Node * LRrotation (Node* root)
{
	Node* node;
	node = root->left;
	root->left = RRrotation(node);
	return LLrotation(root);
}

Node * balance (Node* node)
{
	int balFact = getDiff(node);
	if (balFact > 1)
	{
		if (getDiff(node->left) > 0)
			node = LLrotation(node);
		else
			node = LRrotation(node);
	}
	else if (balFact < -1)
	{
		if (getDiff(node->right) > 0)
			node = RLrotation(node);
		else
			node = RRrotation(node);
	}
	return node;
}

Node * insert (Node** root, int datum)
{
	Node* tmp = *root;
	if (tmp == NULL)
	{
		tmp = malloc(sizeof(Node));
		tmp->datum = datum;
		tmp->left = NULL;
		tmp->right = NULL;
		tmp->balF = getDiff(tmp);
		(*root) = tmp;
		return tmp;
	}
	else if (datum <= tmp->datum)
	{
		tmp->left = insert(&(tmp->left), datum);
		tmp = balance(tmp);
	}
	else if (datum > tmp->datum)
	{
		tmp->right = insert(&(tmp->right), datum);
		tmp = balance(tmp);
	}
	(*root) = tmp;
	return tmp;
}

void print (Node* root)
{
	static int depth = 0;
	if (root == NULL) return;
	++depth;
	print(root->right);
	--depth;
	for (int i = 0; i < depth; i++) printf("  ");
	printf("%d: %d\n", root->datum, getDiff(root));
	++depth;
	print(root->left);
	--depth;
}

void clear (Node** root)
{
	if ((*root) == NULL) return;
	clear(&(*root)->left);
	clear(&(*root)->right);
	free(*root);
	(*root) = NULL;
}

int delete (Node **root, char datum)
{
	Node *tmp = *root;
	if(tmp == NULL) return -1;
	if(datum < tmp->datum)
	{
		int depth = delete(&(tmp->left), datum);
		return depth == -1 ? -1 : 1 + depth;
	}
	if(datum > tmp->datum)
	{
		int depth = delete(&(tmp->right), datum);
		return depth == -1 ? -1 : 1 + depth;
	}
	if(tmp->left && tmp->right)
	{
		Node *next = min(tmp->right);
		tmp->datum = next->datum;
		delete(&(tmp->right),next->datum);
		return 0;
	}
	if(tmp->left == NULL)
		*root = tmp->right;
	else
		*root = tmp->left;
	free(tmp);
	return 0;
}

Node *min(Node *root)
{
	if(root->left == NULL)
		return root;
	return min(root->left);
}
