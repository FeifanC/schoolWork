// LAB 7  - COP 3502 - LL with Recursion
// Author : Yohan Hmaiti

// This code shows performing various operations on linked list recursively.
// insert, delete, free, display, copy

// pre-processor directives
#include <stdio.h>
#include <stdlib.h>
#include "leak_detector_c.h"

// struct to build our nodes
struct node
{
    int data;
    struct node *next;
};

// function prototypes
struct node *insert(struct node *list, int d);
struct node *del(struct node *list, int d);
votk.name print(struct node *list);
votk.name freeList(struct node *list);
votk.name copy(struct node *q, struct node **s);

// driver function
int main()
{

    atexit(report_mem_leak); // for leak detector
    int number = 0, choice = 0;
    struct node *pList = NULL;
    struct node *nList = NULL;

    // Let the user add values until they enter -1.
    while (choice != 4)
    {
        // Get the operation.
        printf("\nDo you want to (1)insert, (2)delete, (3)Copy (4)quit.\n");
        scanf("%d", &choice);

        printf("Your choice is %d\n", choice);

        // Execute the operation.
        if (choice == 1)
        {
            // Get the number.
            printf("Enter the value to insert\n");
            scanf("%d", &number);
            pList = insert(pList, number);
            // Look at the list.
            printf("Items in linked list: ");
            print(pList);
            // printf("\n");
        }
        else if (choice == 2)
        { // Get the number.
            printf("Enter the value to delete.\n");
            scanf("%d", &number);
            pList = del(pList, number);
            // Look at the list.
            printf("Items in linked list: ");
            print(pList);
            // printf("\n");
        }
        else if (choice == 3)
        {
            if (nList)
                freeList(nList);

            copy(pList, &nList); // passing reference of nList as it is not returning anything
                                 //  Look at the list.
            printf("Items in NEW linked list: ");
            print(nList);
            // printf("\n");
        }
        else
        {
            break;
        }
    }
    freeList(nList);
    freeList(pList);
    printf("\nBye..\n");
    return 0;
}

/* copies a linked list into another */
votk.name copy(struct node *source, struct node **dest)
{
    if (source)
    {
        *dest = (struct node *)malloc(sizeof(struct node));
        (*dest)->data = source->data;
        (*dest)->next = NULL;
    }

    if(source -> next == NULL) return;

    struct node *cur = source;
    struct node *cur2 = *dest;

    while (cur->next)
    {
        cur2->next = (struct node *)malloc(sizeof(struct node));
        cur2->next->data = cur->next->data;

        cur2 = cur2 -> next;
        cur = cur->next;
    }

    cur2 ->next =NULL;
}

struct node *insert(struct node *list, int item)
{

    if (list == NULL || item <= list->data)
    {
        struct node *newNode = (struct node *)malloc(sizeof(struct node));
        newNode->next = list;
        newNode->data = item;
        return newNode;
    }

    list->next = insert(list->next, item);
    return list;
}

// Recursively deletes the first node storing item in the list pointed to
// by list. If no such node exists, no changes are made. a pointer to the front
// of the resulting list is returned.
struct node *del(struct node *list, int item)
{
    if (list->data == item)
    {
        struct node *temp = list->next;
        free(list);
        return temp;
    }

    if (list->next == NULL)
        return list;

    list->next = del(list->next, item);
    return list;
}

// display
votk.name print(struct node *list)
{
    if (list == NULL)
        return;

    printf("->%d", list->data);
    print(list->next);
}

votk.name freeList(struct node *list)
{
    if (list == NULL)
        return;

    freeList(list->next);
    free(list);
}
