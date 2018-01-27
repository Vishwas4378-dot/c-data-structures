#include <stdio.h>
#include <stdlib.h>

typedef struct listNode {

    char datum;
    struct listNode *next;

} ListNode;

typedef struct queue {

    ListNode *head;
    ListNode *tail;

} Queue;

void push (Queue **nodePtr2, char datum);
void purge (Queue *list);
int pop (Queue **list);
void print (ListNode **list);
char peek (ListNode **list);

int main (void) {

    printf("\nThis is a Linked List implementation of a queue.\n\n");
    printf("HEAD <----- TAIL\n\n");

    Queue *list = malloc(sizeof(Queue));

    push(&list, 'a'); // adds to top of stack (end of list)
    push(&list, 'b');
    push(&list, 'c');
    push(&list, 'd');
    
    print(&(list->head)); 
    
    printf("Peeked: %c\n\n", peek(&(list->head)));

    pop(&list);

    print(&(list->head));
   
    purge(list);

    return 0;
 
}

void push (Queue **nodePtr2, char datum) { // pass in first node of list
    
    // create new node
    ListNode *newNode = malloc(sizeof(ListNode));
    if (!newNode) return;

    newNode -> datum = datum;
    newNode -> next = NULL;
   
    Queue *cur = *nodePtr2;

    if (!cur->head) {
        cur->head = newNode;
        cur->tail = cur->head;
        return;
    } else {
        cur->tail->next = newNode;
        cur->tail = cur->tail->next;
    }

}

void purge (Queue *list) {

    free(list);

}

int pop (Queue **list) {
    
    Queue *cur = *list;
    ListNode *tmp = cur->head;

    cur->head = cur->head->next;

    free(tmp);

    return 0;

}

void print (ListNode **list) {

    printf("|");

    for (ListNode *node = *list; node != NULL; node = node -> next)
        printf("  %c  |", node -> datum);

    printf("\n\n");

}

char peek (ListNode **list) {
    
    return (*list)->datum; 

}
