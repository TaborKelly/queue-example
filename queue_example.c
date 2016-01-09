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
// head_t without the compiler comlaining.
typedef TAILQ_HEAD(head_s, node) head_t;

// Takes a string and puts in in the queue on character at a time.
static void _fill_list(head_t * head, const char * string)
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
static void _free_list(head_t * head)
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

// Prints the queue by traversing the list forwards.
static void _print_list(head_t * head)
{
    struct node * e = NULL;
    TAILQ_FOREACH(e, head, nodes)
    {
        printf("%c", e->c);
    }
}

// Prints the queue while traversing the list backwards.
static void _print_list_backwards(head_t * head)
{
    struct node * e = NULL;
    struct node * next = NULL;
    TAILQ_FOREACH_REVERSE_SAFE(e, head, head_s, nodes, next)
    {
        printf("%c", e->c);
    }
}

// Safely removes the vowels from the list
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

    // fill the list with "Hello World\n"
    _fill_list(&head, "Hello World!\n");

    printf("Forwards: ");
    _print_list(&head); // prints "Hello World!\n"
    printf("Backwards: ");
    _print_list_backwards(&head); // prints "\n!dlroW olleH"

    // remove the vowels from the list
    _remove_vowels(&head);
    printf("\nForwards, vowels removed: ");
    _print_list(&head); // prints "Hll Wrld!\n"

    // free the list
    _free_list(&head);
    _print_list(&head); // prints ""

    return EXIT_SUCCESS;
}
