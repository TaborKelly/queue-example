// gcc queue_example.c -o queue_example

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h" // queue taken from FreeBSD 10

// Note the standard typedef of struct node to node_t for convenient use but
// the TAILQ_ENTRY macro uses the struct name.
typedef struct node
{
    char c;
    TAILQ_ENTRY(node) nodes;
} node_t;
// This typedef creates a head_t that makes it easy for us to pass pointers to
// head_t without the compiler complaining.
typedef TAILQ_HEAD(head_s, node) head_t;

// Takes a string and puts in in the queue on character at a time.
static void _fill_queue(head_t * head, const char * string)
{
    int c = 0;
    for (c = 0; c < strlen(string); ++c)
    {
        struct node * e = malloc(sizeof(struct node));
        if (e == NULL)
        {
            fprintf(stderr, "malloc failed");
            exit(EXIT_FAILURE);
        }
        e->c = string[c];
        TAILQ_INSERT_TAIL(head, e, nodes);
        e = NULL;
    }
}

// Removes all of the elements from the queue before free()ing them.
static void _free_queue(head_t * head)
{
    struct node * e = NULL;
    while (!TAILQ_EMPTY(head))
    {
        e = TAILQ_FIRST(head);
        TAILQ_REMOVE(head, e, nodes);
        free(e);
        e = NULL;
    }
}

// Prints the queue by traversing the queue forwards.
static void _print_queue(head_t * head)
{
    struct node * e = NULL;
    TAILQ_FOREACH(e, head, nodes)
    {
        printf("%c", e->c);
    }
}

// Prints the queue while traversing the queue backwards.
static void _print_queue_backwards(head_t * head)
{
    struct node * e = NULL;
    TAILQ_FOREACH_REVERSE(e, head, head_s, nodes)
    {
        printf("%c", e->c);
    }
}

// Safely removes the vowels from the queue
static void _remove_vowels(head_t * head)
{
    struct node * e = NULL;
    struct node * next = NULL;
    TAILQ_FOREACH_SAFE(e, head, nodes, next)
    {
        if (e->c == 'a' || e->c == 'A' || e->c == 'e' || e->c == 'E' ||
            e->c == 'i' || e->c == 'I' || e->c == 'o' || e->c == 'O' ||
            e->c == 'u' || e->c == 'U')
        {
            TAILQ_REMOVE(head, e, nodes);
            free(e);
            e = NULL;
        }
    }
}

int main (int arc, char * argv [])
{
    // declare the head
    head_t head;
    TAILQ_INIT(&head); // initialize the head

    // fill the queue with "Hello World\n"
    _fill_queue(&head, "Hello World!\n");

    printf("Forwards: ");
    _print_queue(&head); // prints "Hello World!\n"
    printf("Backwards: ");
    _print_queue_backwards(&head); // prints "\n!dlroW olleH"

    // remove the vowels from the queue
    _remove_vowels(&head);
    printf("\nForwards, vowels removed: ");
    _print_queue(&head); // prints "Hll Wrld!\n"

    // free the queue
    _free_queue(&head);
    _print_queue(&head); // prints ""

    return EXIT_SUCCESS;
}
