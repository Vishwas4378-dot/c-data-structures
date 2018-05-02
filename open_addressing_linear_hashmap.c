#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DEFAULT_SIZE 3

typedef struct node
{
    char *key;
    size_t hash;
    char *value;
} Node;

typedef struct hashmap
{
    Node **array;
    size_t capacity;
    size_t filled;
    unsigned long resizes;
    unsigned long collisions;
} Hashmap;

size_t hash (char *string);
Hashmap * create ();
size_t insert (Hashmap *hm, char *key, char *val);
void resizeIfAlmostFull (Hashmap *hm);
void print (Hashmap *hm);
void clear (Hashmap **hm);
bool isPrime (unsigned long n);
unsigned long power (unsigned long a, unsigned long b, unsigned long mod);
Node * find (Hashmap *hm, char *key);
char * get (Hashmap *hm, char *key);
void delete (Hashmap *hm, char *key);

int main (void)
{
    Hashmap *hm = create();
    
    printf("\nInitial Hashmap of size %zu:\n\n", hm->capacity);
    print(hm);
    printf("\n--------------\n\n");

    insert(hm, "dog", "woof");
    insert(hm, "cat", "meow");
    insert(hm, "bird", "tweet");
    insert(hm, "mouse", "squeak");
    insert(hm, "cow", "moo");
    insert(hm, "frog", "croak");
    insert(hm, "elephant", "toot");
    insert(hm, "ducks", "quack");
    insert(hm, "fish", "blub");
    insert(hm, "seal", "ow, ow, ow");
    insert(hm, "seal", "ow, ow");
   
    print(hm);
    printf("\n--------------\n\n");
    
    delete(hm, "abc");
    delete(hm, "cow");
    print(hm);
    insert(hm, "cow", "moo");
    print(hm);
    
    printf("%s, %s\n", get(hm, "owo"), get(hm, "uwu"));
    printf("%s\n", get(hm, "abc") ? get(hm, "abc") : "Not Found");
    printf("%s\n", get(hm, "seal"));
    
    printf("R: %lu, C: %lu, Filled: %lu, Capacity: %lu\n", hm->resizes, hm->collisions, hm->filled, hm->capacity);
    clear(&hm);
    return 0;
}

size_t hash (char *string)
{
    int num = 31;
    size_t out = 0;
    size_t tmp = 0;

    for(size_t i = 0; string[i] != '\0'; i++)
    {
        tmp = string[i] + tmp*num;
        out += tmp;
    }
    return out;
}

Hashmap *create ()
{
    Hashmap *hm = malloc(sizeof(Hashmap));
    hm->capacity = DEFAULT_SIZE;
    hm->filled = 0;
    hm->resizes = 0;
    hm->collisions = 0;
    hm->array = calloc(hm->capacity, sizeof(Node *));
    return hm;
}

size_t insert (Hashmap *hm, char *key, char *val)
{
    Node *found = find(hm,key);
    if (found)
    {
        found->value = realloc(found->value, sizeof(char)*(strlen(val)+1));
        memcpy(found->value, val, sizeof(char)*(strlen(val)+1));
        return hash(key);
    }
    resizeIfAlmostFull(hm);
    size_t hashedKey = hash(key);
    Node *new = malloc(sizeof(Node));
    new->key = malloc(sizeof(char) * (strlen(key) + 1));
    memcpy(new->key, key, sizeof(char) * (strlen(key) + 1));
    new->value = malloc(sizeof(char) * (strlen(val) + 1));
    memcpy(new->value, val, sizeof(char) * (strlen(val) + 1));
    new->hash = hashedKey;
    size_t index = hashedKey % hm->capacity;

    // need to increment through the list until a spot is found
    for (size_t i = index;; ++i)
    {
        if (hm->array[i % hm->capacity] == NULL || strcmp(hm->array[i % hm->capacity]->value, "AVAIL") == 0)
        {
            hm->array[i % hm->capacity] = new;
            hm->filled++;
            if (i != index) // collision occured
                hm->collisions++;
            break;
        }
    }

    return hashedKey;
}

void resizeIfAlmostFull (Hashmap *hm)
{
    if (hm->capacity * 0.7 < hm->filled)
    {   
        size_t i;
        size_t oldCap = hm->capacity;
        for(i = hm->capacity * 2 + 1; !isPrime(i); i += 2) {}
        hm->capacity = i;
        Node **tmp = calloc(hm->capacity, sizeof(Node *));
        hm->resizes++;
        for (i = 0; i < oldCap; i++)
            if (hm->array[i] != NULL)
                tmp[i] = hm->array[i];
        free(hm->array);
        hm->array = tmp;
    }
}

void print (Hashmap *hm)
{
    Node *cur;
    for(size_t i = 0; i < hm->capacity; i++)
    {
        cur = hm->array[i];
        if (cur)
            printf("K: %s V: %s\n", cur->key, cur->value);
        else
            printf("NULL\n");
    }
}

void clear (Hashmap **hm)
{
    free((*hm)->array);
    free(*hm);
    *hm = NULL;
}

bool isPrime (unsigned long n)
{
    unsigned long a, s, d, r;
    s = 0;
    d = n-1;
    unsigned long witnesses[] = {2, 3, 5, 7, 11};
    for (a = 0; a < 5; a++) 
        if(witnesses[a] == n)
            return true;
    while (d % 2 == 0)
    {
        d /= 2;
        s++;
    }
    for (a = 0; a < 5; a++)
    {
        if ((unsigned long)power(witnesses[a], d, n) % n == 1)
            continue;
        for (r = 0; r < s; r++)
            if ((unsigned long)power(witnesses[a], (unsigned long)power(2, r, n)*d, n) % n == n-1)
                break;
        if (r == s)
            return false;
    }
    return true;
}

unsigned long power (unsigned long a, unsigned long b, unsigned long mod)
{
    if (b == 1)
        return a % mod;
    if (b == 0)
        return 1;
    if (b % 2 == 0)
        return power(a*a % mod, b/2, mod);
    return a * power((a*a) % mod, b/2, mod) % mod;
}

char *get (Hashmap *hm, char *key)
{
    Node *found = find(hm,key);
    return found ? found->value : NULL;
}

Node * find (Hashmap *hm, char *key)
{
    for (size_t i = 0; i < hm->capacity;)
    {
        if (hm->array[i] == NULL)
            ++i;
        else if (strcmp(hm->array[i]->key, key) == 0) // if the keys are the same, return the Node
            return hm->array[i];
        else
            ++i;
    }
    return NULL;
}

void delete (Hashmap *hm, char *key)
{
    Node *found = find(hm, key);
    Node *new = malloc(sizeof(Node *));
    if (found)
    {
        new->value = "AVAIL";
        new->key = "AVAIL";
        new->hash = hash(key);
        hm->array[hash(key) % hm->capacity] = new;
    }
}
