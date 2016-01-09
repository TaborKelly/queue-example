// gcc simple_queue_example.c -o simple_queue_example

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h" // queue taken from FreeBSD 10

// The data type for the node
struct node
{
    char c;
    // This macro does the magic to point to other nodes
    TAILQ_ENTRY(node) nodes;
};

int main (int arc, char * argv [])
{
    // This macro creates the data type for the head of the queue
    // for nodes of type 'struct node'
    TAILQ_HEAD(head_s, node) head;
    // Initialize the head before use
    TAILQ_INIT(&head);
    char string[] = "Hello World\n";

    //
    // put some nodes in the queue
    //

    struct node * e = NULL;
    int c = 0;
    for (c = 0; c < strlen(string); ++c)
    {
        e = malloc(sizeof(struct node));
        if (e == NULL)
        {
            fprintf(stderr, "malloc failed");
            exit(EXIT_FAILURE);
        }
        e->c = string[c];
        // Actually insert the node e into the queue at the end
        TAILQ_INSERT_TAIL(&head, e, nodes);
        e = NULL;
    }

    // print the queue
    TAILQ_FOREACH(e, &head, nodes)
    {
        printf("%c", e->c);
    }

    // free the elements from the queue
    while (!TAILQ_EMPTY(&head))
    {
        e = TAILQ_FIRST(&head);
        TAILQ_REMOVE(&head, e, nodes);
        free(e);
        e = NULL;
    }

    return EXIT_SUCCESS;
}
