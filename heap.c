#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CAPACITY 1 
#define PRINT_TAB 4

typedef struct heap
{
    size_t capacity;
    size_t size;
    unsigned int *data;
} Heap;

Heap * create ();
void destroy (Heap *heap);
void push (Heap *heap, unsigned int priority);
void resize (Heap *heap);
void print (Heap *heap);
unsigned int peek (Heap *heap);
unsigned int pop (Heap *heap);
Heap * initialize (unsigned int list[], size_t); //takes in array and creates heap from it
Heap * merge (Heap *heap1, Heap *heap2); // merges heap1 and heap2
void sift (Heap *heap, size_t index);

int main (void)
{
    unsigned int val[10] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    Heap *heap = initialize(val, 10);
    Heap *heap2 = initialize(val, 10);
    Heap *heap3 = merge(heap, heap2);
    print(heap3);
    unsigned int l[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Heap *heap4 = create();
    push(heap4, 1);
    push(heap4, 2);
    push(heap4, 3);
    push(heap4, 4);
    push(heap4, 5);
    print(heap4);
    printf("Peeked: %d\n", peek(heap4));
    printf("Popped: %d\n", pop(heap4));
    print(heap4);
    destroy(heap);
    destroy(heap2);
    destroy(heap3);
    destroy(heap4);
    return 0;
}

Heap * create ()
{
    Heap *heap = malloc(sizeof(Heap));
    heap->size = 0;
    heap->capacity = CAPACITY;
    heap->data = calloc(CAPACITY, sizeof(unsigned int));    
    return heap;
}

void destroy (Heap *heap)
{
    free(heap->data);
    heap->data = calloc(heap->capacity, sizeof(unsigned int));
    heap->size = 0;
    heap->capacity = 0;
    free(heap->data);
    free(heap);
}

void push (Heap *heap, unsigned int priority) // this is what's changed for priority queues
{
    resize(heap);
    heap->data[heap->size++] = priority;

    for (size_t i = heap->size - 1, pi = (i - 1) / 2; i > 0 && heap->data[pi] < heap->data[i]; i = pi, pi = (pi - 1) / 2)
    {
        unsigned int tmp = heap->data[i];
        heap->data[i] = heap->data[pi];
        heap->data[pi] = tmp;
    }
}

void resize (Heap *heap)
{
    if (heap->size >= heap->capacity)
    {
        heap->capacity *= 2;
        heap->data = realloc(heap->data, sizeof(unsigned int) * heap->capacity);
    }
}

void print (Heap *pq) {
    for (size_t i = 0, count = 0; i < pq->size; count++) {
        // Print out layer
        for (size_t j = 0; j < count; j++) {
            for (int k = 0; k < PRINT_TAB; k++)
                printf(" ");
        }
        while (ceil(log2(i+2)) != floor(log2(i+2))) {
            if (i >= pq->size) {
                puts("\n");
                return;
            }
            printf("%d ", pq->data[i]);
            i++;
        }
        if (i == pq->size) {
            puts("");
            return;
        }
        printf("%d\n", pq->data[i]);
        i++;
    }
    puts("");
}

/*void print (Heap *heap)
{
    printf("|");
    
    for (size_t i = 0; i < heap->size; ++i)
        printf("  %u  |", heap->data[i]);

    printf("\n\n");
}*/

unsigned int peek (Heap *heap)
{
    return heap->data[0];
}

unsigned int pop (Heap *heap)
{
    unsigned int out = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    sift(heap, 0);
    return out;
}

Heap * initialize (unsigned int list[], size_t size)
{
    Heap *heap = malloc(sizeof(Heap));
    heap->size = size;
    heap->capacity = size * 2;
    heap->data = calloc(heap->capacity, sizeof(unsigned int));
    memcpy(heap->data, list, sizeof(unsigned int) * size);
    for (size_t i = size/2-1; (int)i >= 0; --i)
        sift(heap, i);
    return heap;
}

Heap * merge (Heap *heap1, Heap *heap2)
{
    unsigned int tmp[heap1->size + heap2->size];
    memcpy(tmp, heap1->data, sizeof(unsigned int) * heap1->size);
    memcpy(tmp + heap1->size, heap2->data, sizeof(unsigned int) * heap2->size);
    return initialize(tmp, heap1->size + heap2->size);
}

void sift (Heap *heap, size_t index)
{
    size_t max = 0;
    unsigned int tmp;
    for (size_t i = index; i*2+1 < heap->size; i = max)
    {
        if (heap->data[max] < heap->data[i*2+1])
            max = i*2+1;
        if (i*2+2 < heap->size && heap->data[max] < heap->data[i*2+2])
            max = i*2+2;
        if (max == i)
            break;
        else
        {
            tmp = heap->data[i];
            heap->data[i] = heap->data[max];
            heap->data[max] = tmp;
        }
    }
}
