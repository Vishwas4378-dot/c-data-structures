#include <stdio.h>
#include <stdlib.h>

#define CAPACITY 8

typedef struct Vector {

    size_t capacity;
    size_t size;
    char *data;

} Vector;

void prepare (Vector *vector);
void purge (Vector *vector);
void push (Vector *vector, char datum);
void polymorph (Vector *vector);
void print (Vector *vector);
char peek (Vector *vector);
void pop (Vector *vector);

int main (void) {

    Vector *list = malloc(sizeof(char));
    prepare(list);

    printf("\nThis is an Array List implementation of a stack.\n\n");

    push(list, 'a');
    push(list, 'b');
    push(list, 'c');
    print(list);
    printf("Peeked: %c\n\n", peek(list)); 
    pop(list);
    print(list);
    purge(list);
    return 0;

}

void prepare (Vector *vector) {

    vector->size = 0;
    vector->capacity = CAPACITY;
    vector->data = calloc(CAPACITY, sizeof(char));

}

void purge (Vector *vector) {

    free (vector->data);

}

void push (Vector *vector, char datum) {

    polymorph(vector);
    vector->data[vector->size++] = datum;

}

void polymorph (Vector *vector) {

    if (vector->size >= vector->capacity) {
        vector->capacity *= 2;
        vector->data = realloc(vector->data, sizeof(char) * vector->capacity);
        printf("resized\n");
    }

}

void print (Vector *vector) {

    printf("|");

    for (size_t i = 0; i < vector->size; ++i) {
        printf("  %c  |", vector->data[i]);
    } 

    printf("\n\n");

}

char peek (Vector *vector) {

    return vector->data[vector->size-1];

}

void pop (Vector *vector) {

    vector->data[vector->size] = 0;
    vector->size--;

}
