#include <stdio.h>
#include <stdlib.h>

typedef struct node {

    char datum;
    struct node *left;
    struct node *right;

} Node;

// should be recursive
int insert (Node **root, char datum); // return depth
void clear (Node **root);
void printTree (Node *root);
int find (Node *tree, char datum); // return depth
int delete (Node **tree, char datum); // return depth found
Node * rightMost(Node *tree);

int main (void) {
    
    Node *root = NULL;
    
    insert(&root, 'k');
    insert(&root, 'a');
    insert(&root, 'a');
    insert(&root, 'b');
    insert(&root, 'z');

    printTree(root);
    printf("\n\n");
    printf("depth of a: %d\n", find(root, 'a'));

    delete(&root, 'k');
 
    printf("deleted k\n");
    
    printf("depth of a: %d\n", find(root, 'a'));
    
    printTree(root);

    clear(&root);

    return 0;

}

int insert (Node **root, char datum) {

    if (*root == NULL) {
        Node *child = malloc(sizeof(Node));
        child->datum = datum;
        child->left = NULL;
        child->right = NULL;
        *root = child;
        return 0;
    } else {
        if (datum > (*root)->datum) {
            return 1 + insert(&((*root)->right), datum);
        } else {
            return 1 + insert(&((*root)->left), datum);
        }
    }

}

void clear (Node **root) {

    if (!*root) {
        return;
    } else {
       clear(&((*root)->left));
       clear(&((*root)->right));
       free(*root);
    }

}

void printTree (Node *first) {

    static int depth = 0;

    if (first == NULL) {
        depth = 0; 
    } else {
        int tmp = depth;
        ++depth;
        if (first->right != NULL) {
            printTree(first->right);
            for (int i = 0; i < tmp+2; ++i) {
                printf(" ");
            }
            puts("|");
        }
        depth = tmp + 1;
        for (int k = 0; k < depth; ++k) {
            printf(" ");
        }
        printf("-%c\n", first->datum);
        depth = tmp+1;
        if (first->left != NULL) { 
            for (int i = 0; i < tmp+2;++i) {
                printf(" ");
            }
            puts("|");
            printTree(first->left);
        }
        depth = tmp;
    }

}

int find (Node *root, char datum) {

    if (root == NULL) {
       return -1;
    } else if (root->datum == datum) {
        return 0;
    } else {
        int out;
        if (datum < root->datum) {
            out = find(root->left, datum);
        } else {
            out = find(root->right, datum);
        }
        if (out == -1) {
            return -1;
        } else {
            return 1 + out;
        }
    }

}

int delete (Node **tree, char datum) {

    Node *tmp = *tree;
    if (tmp == NULL)
        return 0;
    if (tmp->datum > datum) {
        return delete(&(tmp->left), datum);
    }
    if (tmp->datum < datum) {
        return delete(&(tmp->right), datum);
    }
    if (tmp->datum == datum) {
        if (tmp->left == NULL && tmp->right == NULL) {
            free(tmp);
            tmp = NULL;
            return 1;
        }
        
        if ((tmp->left == NULL) ^ (tmp->right == NULL)) {
            Node *only = tmp->left ? tmp->left : tmp->right;
            tmp->datum = only->datum;
            tmp->left = only->left;
            tmp->right = only->right;
            free(only);
            return 1;
        }

        Node *rightest = rightMost((*tree)->left);
        if(rightest == (*tree)->left) {
            Node *newNode = malloc(sizeof(Node));
            newNode->datum = rightest->datum;
            newNode->right = (*tree)->right;
            newNode->left = (*tree)->left;
            tmp = newNode;
            free(tmp);
            delete(&(tmp->right), rightest->datum);
        }
        (*tree)->datum = rightest->datum;
        delete(&(tmp->right), rightest->datum);
        return 1;
    }

    return 0;

}

Node * rightMost (Node *tree) {

    if (tree->right == NULL) {
        return tree;
    }

    return rightMost(tree->right);

}
