#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef enum type {
	FLOAT,
	INT,
	OPERATOR
} Type;

union datum {
	char c;
	double d;
	int i;
};

typedef struct node {
	union datum datum;
	struct node *left;
	struct node *right;
	Type type;
} Node;

typedef struct tokens {
	size_t length;
	char **list;
} Tokens;

//int insert(Node **root, char datum);
void clear (Node **root);
//int find(Node *root, char datum);
void print (Node *root);
//int delete(Node **root, char datum);
//Node *min(Node *root);
//void parse(char *string, Node **tree);
//char* leastSigOp(char **tree);
Tokens tokenize (char *string);
Type getTypeChar (char datum);
Type getTypeStr (char *datum);
size_t getNextToken (char *string);
Node *parse (char **tokens, size_t length);
bool isOpen (char *token);
bool isClosed (char *token);
Node * evaluate (Node *tree);

int main (int argc, char *argv[]) {

	Tokens tokens = tokenize(argv[1]);
	Node *tree = parse(tokens.list, tokens.length);
	print(tree);
    tree = evaluate(tree);
	print(tree);
    clear(&tree);
	return 0;

}

// int insert(Node **root, char datum){
// 	Node *tmp = *root;
// 	if(tmp == NULL){
// 		*root = malloc(sizeof(Node));
// 		tmp = *root;
// 		tmp->datum = datum;
// 		tmp->left = NULL;
// 		tmp->right = NULL;
// 		return 0;
// 	}
// 	if(datum < tmp->datum){
// 		return 1 + insert(&(tmp->left), datum);
// 	}
// 	return 1 + insert(&(tmp->right), datum);
// }

void clear (Node **root) {

	Node *tmp = *root;

	if(tmp == NULL) return;

	clear(&(tmp->left));
	clear(&(tmp->right));
	free(tmp);
	*root = NULL;

}

// int find(Node *root, char datum){
// 	if(root == NULL){
// 		return -1;
// 	}
// 	if(datum == root->datum){
// 		return 0;
// 	}
// 	int depth;
// 	if(datum < root->datum){
// 		depth = find(root->left, datum);
// 	}
// 	else{
// 		depth = find(root->right, datum);
// 	}
// 	return depth == -1 ? -1 : 1 + depth;
// }

void print(Node *root) {

	static int depth = 0;
	if(root == NULL) return;
	++depth;
	print(root->right);
	--depth;

	for (int i = 0; i < depth; ++i) {
		printf("  ");
	}

	switch (root->type) {
		case OPERATOR:
			printf("%c\n", root->datum.c);
			break;
		case FLOAT:
			printf("%f\n", root->datum.d);
			break;
		default:
			printf("%d\n", root->datum.i);
	}

	++depth;
	print(root->left);
	--depth;

}

// int delete(Node **root, char datum){
// 	Node *tmp = *root;
// 	if(tmp == NULL)
// 		return -1;
// 	if(datum < tmp->datum){
// 		int depth = delete(&(tmp->left), datum);
// 		return depth == -1 ? -1 : 1 + depth;
// 	}
// 	if(datum > tmp->datum){
// 		int depth = delete(&(tmp->right), datum);
// 		return depth == -1 ? -1 : 1 + depth;
// 	}
// 	if(tmp->left && tmp->right){
// 		Node *next = min(tmp->right);
// 		tmp->datum = next->datum;
// 		delete(&(tmp->right),next->datum);
// 		return 0;
// 	}
// 	if(tmp->left == NULL){
// 		*root = tmp->right;
// 	}
// 	else{ //if(tmp->right == NULL)
// 		*root = tmp->left;
// 	}
// 	free(tmp);
// 	return 0;
// }

// Node *min(Node *root){
// 	if(root->left == NULL){
// 		return root;
// 	}
// 	return min(root->left);
// }
/*
void parse(char *string, Node **tree) {
	Node *new = malloc(sizeof(Node));
	if(!*tree)
		*tree = new;
}

char* leastSigOp(char **tree) {
	char ***order = {{"+", "-"}, {"x", "/", "%"}, {"^"}};
	for( int i = 0; i <
}
*/

Tokens tokenize (char *string) {

	size_t count = 1;
	Type flag = getTypeChar(string[0]);
	for (size_t i = 1; string[i] != 0; i++) {
		if (getTypeChar(string[i]) != flag) {
			count++;
			flag = getTypeChar(string[i]);
		} else if (flag == OPERATOR) {
			count++;
		}
        if (isOpen(&string[i])) {
            if (getTypeChar(string[i-1]) != OPERATOR || isClosed(&string[i-1]))
            {
                ++count;
            } 
        }
        else if (isClosed(&string[i-1]) && getTypeChar(string[i]) != OPERATOR) {
            ++count;
        } 
	}
  
  char **tokens = malloc(sizeof(char *) * (count + 1));
  size_t tmp = 0, strSize;
  char *token;

  for (size_t i = 0; i < count; i++) {
    
    if (isOpen(&string[tmp])) {
        if (getTypeChar(string[tmp-1]) != OPERATOR || isClosed(&string[tmp-1])) {
            token = malloc(sizeof(char) * 2);
            strncpy(token, "x", 2);
            tokens[i] = token;
            ++i;
        } 
    }
    else if (isClosed(&string[tmp-1]) && getTypeChar(string[tmp]) != OPERATOR) {
        token = malloc(sizeof(char) * 2);
        strncpy(token, "x", 2);
        tokens[i] = token;
        ++i;
    } 
    strSize = getNextToken(string + tmp);
    token = malloc(sizeof(char) * (strSize + 1));
    strncpy(token, string + tmp, strSize);
    token[strSize] = '\0';
    tokens[i] = token;
    tmp += strSize;
  }

    token = malloc(sizeof(char));
	token[0] = '\0';
	tokens[count] = token;

	Tokens *tokes = malloc(sizeof(Tokens));
	tokes->list = tokens;
	tokes->length = count;
	return *tokes;

}

Type getTypeChar (char datum) {

	if((datum >= '0' && datum <= '9') || datum == '.')
		return FLOAT;
	return OPERATOR;

}

Type getTypeStr(char *datum){

	Type out = getTypeChar(datum[0]);
	if (out == OPERATOR) {
		return out;
	}
	for (size_t i = 0; datum[i] != 0; i++) {
		if (datum[i] == '.') {
			return FLOAT;
		}
	}
	return INT;

}

size_t getNextToken (char *string) {

    Type flag = getTypeChar(string[0]);
		if (flag == OPERATOR) {
			return 1;
		}
    size_t i;
    for (i = 1; string[i] && getTypeChar(string[i]) == flag; i++);
    return i;

}

Node *parse (char **tokens, size_t length) {

	if (length == 0) return NULL;
	if (length == 1 && getTypeStr(tokens[0]) != OPERATOR) {
		Node *node = malloc(sizeof(Node));
		node->left = NULL;
		node->right = NULL;
		if (getTypeStr(tokens[0]) == FLOAT) {
			node->datum.d = atof(tokens[0]);
			node->type = FLOAT;
		} else {
			node->datum.i = atoi(tokens[0]);
			node->type = INT;
		}
		return node;
	}

	int count = 0;
	if (isOpen(tokens[0]) && isClosed(tokens[length -1])) {
		for (size_t i=1;i<length-1;i++) {
			if (isOpen(tokens[i])){
				count++;
			} else if (isClosed(tokens[i])) {
				count--;
			}	if(count == -1) {
				break;
			}
		}
		if (count == 0) {
			return parse(tokens+1,length-2);
		}
	}

	char **ptr = NULL;
	count = 0;
	for (size_t i = length-1;i>0;--i) {
		count += isOpen(tokens[i]);
		count -= isClosed(tokens[i]);
		if (count < 0) continue;
		if (tokens[i][0] == '+'||tokens[i][0] == '-') {
			if (tokens[i][0] == '-') {
				if (!(getTypeStr(tokens[i-1]) != OPERATOR || isClosed(tokens[i-1]))) {
					continue;
				}
			}
			ptr = tokens+i;
			break;
		}
	}
	if (!ptr) {
		count = 0;
		for (size_t i = length-1; i > 0; i--) {
			count += isOpen(tokens[i]);
			count -= isClosed(tokens[i]);
			if (count < 0) continue;
			if (tokens[i][0] == 'x' || tokens[i][0] == '%'||tokens[i][0] == '/') {
				ptr = tokens+i;
				break;
			}
		}
	}

	if (!ptr) {
		count = 0;
		for (size_t i = length-1; i > 0; i--) {
			count += isOpen(tokens[i]);
			count -= isClosed(tokens[i]);
			if (count < 0)	continue;
			if (tokens[i][0] == '^') {
				ptr = tokens+i;
				break;
			}
		}
	}

	if (!ptr) {
		count = 0;
		for (size_t i = 0; i < length - 1; ++i) {
			count += isOpen(tokens[i]);
			count -= isClosed(tokens[i]);
			if (count < 0) continue;
			if (tokens[i][0] == '-') {
				ptr = tokens+i;
				break;
			}
		}
	}
	Node *node = malloc(sizeof(Node));
	node->datum.c = ptr[0][0];
	node->left = parse(tokens, ptr-tokens);
	node->right = parse(ptr+1, length-(ptr-tokens)-1);
	node->type = OPERATOR;
	return node;

}

bool isOpen(char *token){

	if (token[0] =='(') {
		return true;
	}

	return false;

}

bool isClosed (char *token) {

	if (token[0] ==')') {
		return true;
	}
	
	return false;

}

Node * evaluate (Node *tree) {
    
    if (tree == NULL) {
        Node *final = malloc(sizeof(Node));
        final->datum.i = 0;
        final->type = INT;
        final->right = NULL;
        final->left = NULL;
        return final;
    }
    if (tree->left == NULL && tree->right == NULL) {
        return tree;
    }
    Node *left = evaluate(tree->left);
    Node *right = evaluate(tree->right);
    double leftVal = left->type == INT ? (double)(left->datum.i) : left->datum.d;
    double rightVal = right->type == INT ? (double)(right->datum.i) : right->datum.d;
    free(tree->left);
    free(tree->right);
    tree->left = NULL;
    tree->right = NULL;
    switch (tree->datum.c) {
        case '^':
            tree->datum.d = pow(leftVal, rightVal);
            break;
        case '+':
            tree->datum.d = leftVal + rightVal;
            break;
        case '-':
            tree->datum.d = leftVal - rightVal;
            break;
        case 'x':
            tree->datum.d = leftVal * rightVal;
            break;
        case '/':
            if (rightVal == 0) {
                printf("DIVIDE BY ZERO ERROR\n");
                exit(-1);
            }
            tree->datum.d = leftVal / rightVal;
            break;
        case '%':
            if (rightVal == 0) {
                printf("DIVIDE BY ZERO ERROR\n");
                exit(-1);
            }
            tree->datum.d = fmod(leftVal, rightVal);
            if (tree->datum.d < 0) {
               tree->datum.c += rightVal;
            }
            break;
        default:
            printf("INVALID OPERATOR\n");
            exit(-1);
    }
    tree->type = FLOAT;
    double rounded = round(tree->datum.d);
    if (fabs((tree->datum.d - rounded) / tree->datum.d) < 1e-4) {
        tree->datum.i = (int)rounded;
        tree->type = INT;
    }

    return tree;

}
