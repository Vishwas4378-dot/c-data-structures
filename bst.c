#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
  char datum;
  struct node *left;
  struct node *right;
} Node;

int insert(Node **root, char datum);
void clear(Node **root);
int find(Node *root, char datum);
void print(Node *root);
int delete(Node **root, char datum);
Node *min(Node *root);

int main()
{
  Node *root = NULL;
  printf("%c inserted at %d\n", 'k', insert(&root, 'k'));
  printf("%c inserted at %d\n", 'a', insert(&root, 'a'));
  printf("%c inserted at %d\n", 'x', insert(&root, 'x'));
  printf("%c inserted at %d\n", 'z', insert(&root, 'z'));
  printf("%c inserted at %d\n", 'l', insert(&root, 'l'));
  printf("%c inserted at %d\n", 'm', insert(&root, 'm'));
  print(root);
  printf("Depth of %c: %d\n", 'k', find(root,'k'));
  printf("Depth of %c: %d\n", 'a', find(root,'a'));
  printf("Depth of %c: %d\n", 'x', find(root,'x'));
  printf("Depth of %c: %d\n", 'z', find(root,'z'));
  printf("Depth of %c: %d\n", 'l', find(root,'l'));
  printf("Depth of %c: %d\n", 'm', find(root,'m'));
  printf("%c deleted at %d\n", 'k', delete(&root,'k'));
  print(root);
  printf("%c deleted at %d\n", 'x', delete(&root,'x'));
  print(root);
  printf("%c deleted at %d\n", 'b', delete(&root,'b'));
  clear(&root);
  print(root);
  return 0;
}

int insert(Node **root, char datum)
{
  Node *tmp = *root;
  if(tmp == NULL)
  {
    *root = malloc(sizeof(Node));
    tmp = *root;
    tmp->datum = datum;
    tmp->left = NULL;
    tmp->right = NULL;
    return 0;
  }
  if(datum < tmp->datum)
  {
    return 1 + insert(&(tmp->left), datum);
  }
  return 1 + insert(&(tmp->right), datum);
}

void clear(Node **root)
{
  Node *tmp = *root;
  if(tmp == NULL)
    return;
  clear(&(tmp->left));
  clear(&(tmp->right));
  free(tmp);
  *root = NULL;
}

int find(Node *root, char datum)
{
  if(root == NULL)
    return -1;
  if(datum == root-> datum)
    return 0;
  int depth;
  if(datum < root->datum)
    depth = find(root->left, datum);
  else
    depth = find(root->right, datum);
  return depth == -1 ? -1 : 1 + depth;
}

void print (Node *first) {

    static int depth = 0;

    if (first == NULL) {
        depth = 0; 
    } else {
        int tmp = depth;
        ++depth;
        if (first->right != NULL) {
            print(first->right);
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
            print(first->left);
        }
        depth = tmp;
    }

}


int delete(Node **root, char datum)
{
  Node *tmp = *root;
  if(tmp == NULL)
    return -1;
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
  else //if(tmp->right == NULL)
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
