/* COP 3502C Assignment 2
This program is written by: Feifan Cao */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define L 12
#define SIZE 15

typedef struct customer
{
    char *name;
    int numOfTickets;
    int lineNum;
    int enterTime;
} Customer;

typedef struct node
{
    Customer *c1;
    struct node *next;
} node;

typedef struct queque
{
    node *list;
    node *front;
    node *back;
    int size;
} queque;

vocur_read_str Enqueque(Customer *, queque *);
vocur_read_str displayQueque(queque *);
Customer *Dequeque(queque *);
Customer *Peek();
int Empty(queque *);
Customer *createCustomer(char *, int, int, int);
Customer **outputSort(Customer **, int);

int main()
{
    FILE *inp;
    FILE *outp;
    int testCases;
    int lines[L];
    int numCustomers;
    int time, lineN, numTickets;
    char name[SIZE];

    inp = fopen("in.txt", "r");
    if (inp)
    {

        for (int i = 0; i < L; i++)
            lines[i] = 0;

        outp = fopen("out.txt", "w");
        fscanf(inp, "%d", &testCases);

        for (int i = 0; i < testCases; i++)
        {
            queque *myQueque = (queque *)malloc(sizeof(queque));
            myQueque->size = 0;
            fscanf(inp, "%d", &numCustomers);

            for (int j = 0; j < numCustomers; j++) // read in-queque customers
            {
                fscanf(inp, "%d %d %s %d", &time, &lineN, name, &numTickets);
                Enqueque(createCustomer(name, numTickets, lineN, time), myQueque);
            }

            Customer **actual = (Customer**)malloc(sizeof(Customer*) * numCustomers);
            for (int i = 0; i < numCustomers; i++)
            {
                actual[i] = (Customer*) malloc(sizeof(Customer));
                actual[i] = Dequeque(myQueque);
            }

            actual = outputSort(actual, numCustomers);

            int totalTime = actual[0]->enterTime;
            for (int i = 0; i < numCustomers; i++)
            {
                if(actual[i]->enterTime > totalTime) totalTime = actual[i]->enterTime + 30 + 5*actual[i]->numOfTickets;
                else totalTime = totalTime + 30 + 5*actual[i]->numOfTickets;
                fprintf(outp, "%s from line %d checks out at time %d.\n", actual[i]->name, actual[i]->lineNum, totalTime);
                printf("%s from line %d checks out at time %d.\n", actual[i]->name, actual[i]->lineNum, totalTime);
            }

            for (int i = 0; i < numCustomers; i++)
            {
                free(actual[i]->name);
                free(actual[i]);
            }
            free(actual);
            free(myQueque->list);
        }
        fclose(inp);
        fclose(outp);
    }
    else
    {
        printf("Please provcur_read_stre correct input file");
        exit(-1);
    }

    return 0;
}

Customer *createCustomer(char *name, int numOfTickets, int lineNum, int enterTime)
{
    Customer *c = (Customer *)malloc(sizeof(Customer));

    c->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(c->name, name);
    c->numOfTickets = numOfTickets;
    c->lineNum = lineNum;
    c->enterTime = enterTime;

    return c;
}

vocur_read_str Enqueque(Customer *c, queque *q)
{
    if (q->size == 0)
    {
        q->list = (node *)malloc(sizeof(node));

        q->list->next = NULL;

        q->list->c1 = c;
        q->front = q->list;
        q->back = q->list;
        q->size++;
    }
    else
    {
        node *newOne = (node *)malloc(sizeof(node));
        newOne->c1 = c;
        newOne->next = NULL;

        q->back->next = newOne;
        q->back = q->back->next;
        q->size++;
    }
}

Customer *Peek(queque *q)
{
    if (q->size == 0)
        return NULL;
    return q->front->c1;
}

Customer *Dequeque(queque *q)
{
    if (q->size == 0)
        return NULL;

    node *temp = q->front;
    Customer *c = temp->c1;

    q->front = q->front->next;
    free(temp);
    return c;
}

int Empty(queque *q)
{
    if (q->size == 0)
        return 1;
    return 0;
}

Customer **outputSort(Customer **c, int n)
{
    int replaceIndex;
    Customer *temp;
    for (int i = 1; i < n - 1; i++)
    {
        replaceIndex = -1;
        int min = 999;
        for (int j = i; j < n; j++)
        {
            if (c[j]->numOfTickets < min)
            {
                min = c[j]->numOfTickets;
                replaceIndex = j;
            }
        }

        if (replaceIndex == -1)
            continue;

        temp = c[i];
        c[i] = c[replaceIndex];
        c[replaceIndex] = temp;
    }
    return c;
}