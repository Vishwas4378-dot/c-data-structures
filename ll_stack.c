#include <stdio.h>
#include <stdlib.h>

typedef struct listNode {

    char datum;
    struct listNode *next;

} ListNode;

typedef struct queue {

    struct listNode *head;

} Queue;

void push (Queue **nodePtr2, char datum);
void purge (Queue *list);
int pop (Queue **list);
void print (ListNode **list);
char peek (Queue **list);

int main (void) {
    
    printf("\nThis is a Linked List implementation of a stack.\n\n");
    
    Queue *list = malloc(sizeof(Queue));

    push(&list, 'a'); // adds to top of stack (end of list)
    push(&list, 'b');
    push(&list, 'c');
    push(&list, 'd');
    
    print(&list->head); 
    
    printf("Peeked: %c\n\n", peek(&list));

    pop(&list);

    print(&list->head);
   
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
        return;
    } else {
        newNode->next = cur->head;
        cur->head = newNode;
    }

    // check for the furthest element in the list
    //while (cur -> next)
        //cur = cur -> next;
    
    // point the previously last element to the new last element
    //cur -> next = newNode;
    return;
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

char peek (Queue **list) {

    return (*list)->head->datum;

}
