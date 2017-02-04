#include <string.h>

typedef struct gaiLinkedListNode
{
    void *data;
    gaiLinkedListNode *prev;
    gaiLinkedListNode *next;
} gaiLinkedListNode;

typedef struct
{
    gaiLinkedListNode *head;
    gaiLinkedListNode *tail;
    i32 size;
} gaiLinkedList;

void
gaiLinkedListCreate(gaiLinkedList *list)
{
    list->head = 0;
    list->tail = 0;
    list->size = 0;
}

void
gaiLinkedListDestroy(gaiLinkedList *list)
{
    gaiLinkedListNode *node = list->head;
    while (node->next)
    {
        gaiLinkedListNode *del_node = node;
        node = del_node->next;

        free(del_node->data);
        free(del_node);
    }
    list->head = 0;
    list->tail = 0;
    list->size = 0;
}

void
gaiLinkedListPrepend(gaiLinkedList *list, void *data, size_t size)
{
    gaiLinkedListNode *new_node = (gaiLinkedListNode *) malloc(sizeof(gaiLinkedListNode));
    new_node->next = 0;
    new_node->prev = 0;
    new_node->data = (void*) malloc(size);
    memset(new_node->data, 0, size);
    memcpy(new_node->data, data, size);

    if (!list->head && !list->tail)
    {
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        gaiLinkedListNode *it = list->head;
        it->prev = new_node;

        list->head = new_node;
        new_node->next = it;
    }
    list->size++;
}


