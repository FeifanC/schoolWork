/* COP 3502C Programming Assignment 5
This program is written by: Feifan Cao */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"
//#include "leak_detector_c.c"

#define LEN 50
char **temp;
int i = 0;

typedef struct heapStruct
{
    char stringArray[10001][LEN*4];
    int capacity;
    int size;
} heap;

heap *initHeap();
void percolateDownMin(heap *h, int index);
void percolateUpMin(heap *h, int index);
void percolateDownMax(heap *h, int index);
void percolateUpMax(heap *h, int index);
int insertForMin(heap *h, char value[]);
int insertForMax(heap *h, char value[]);
char* peek(heap* h);
char *removeMin(heap *h);
char *removeMax(heap* h);
void printheap(heap *h);
void swap(heap *h, int index1, int index2);
void freeheap(heap *h);
int compareTo(char s1[], char s2[]);
void whatHappen(heap* secondHalf, heap* firstHalf, char word[], FILE* f);
void freeTempString(char** strs, int len);

int main()
{

    atexit(report_mem_leak);

    temp = malloc(sizeof(char*) * LEN*1000);
    FILE *inF;
    FILE *outF;
    char word[LEN];
    int count;

    inF = fopen("in.txt", "r");

    if (inF)
    {
        heap *half1 = initHeap();
        heap *half2 = initHeap();
        outF = fopen("out.txt", "w");

        fscanf(inF, "%d", &count);

        for (int i = 0; i < count; i++)
        {
            fscanf(inF, "%s", word);
            whatHappen(half2, half1, word, outF);        

        }
        
        freeheap(half1);
        freeheap(half2);
        freeTempString(temp, LEN*1000);

        fclose(inF);
        fclose(outF);
        
    }
    else
    {
        printf("Please provide correct input file");
        exit(-1);
    }
}

void freeTempString(char** strs, int len)
{
    for (int i = 0; i < len; i++)
    {
        free(strs[i]);
    }
    free(strs);
}

void whatHappen(heap* secondHalf, heap* firstHalf, char word[], FILE* f)
{   
    if(secondHalf->size == 0 && firstHalf ->size == 0)   
        insertForMin(secondHalf, word);
    else{
        if(firstHalf->size > secondHalf->size)
        {
            if(compareTo(word, peek(firstHalf)) > 0)
                insertForMin(secondHalf,word);
            else{
                char *retri = removeMax(firstHalf);
                insertForMax(firstHalf, word);
                insertForMin(secondHalf, retri);
                temp[i++] = retri;
            }
        }else if(firstHalf->size < secondHalf->size)
        {
            if(compareTo(word, peek(secondHalf)) > 0)
            {
                char* retri = removeMin(secondHalf);
                insertForMax(firstHalf,retri);
                insertForMin(secondHalf, word);
                temp[i++] = retri;
            }               
            else
            {
                insertForMax(firstHalf, word);  
            }
        }
        else{
            if(compareTo(word, peek(secondHalf)) > 0)
                insertForMin(secondHalf, word);
            else
                insertForMax(firstHalf, word);
        }
    }

    if(firstHalf->size == secondHalf->size)
    {
        printf("%s %s\n",peek(firstHalf), peek(secondHalf));
        fprintf(f, "%s %s\n",peek(firstHalf), peek(secondHalf));
    }
    else if (firstHalf->size > secondHalf->size){
        printf("%s\n", peek(firstHalf));
        fprintf(f,"%s\n", peek(firstHalf));
    }
    else
    {
        printf("%s\n", peek(secondHalf));
        fprintf(f, "%s\n", peek(secondHalf));
    }
}


int compareTo(char s1[], char s2[])
{
    if (strlen(s1) > strlen(s2))
        return 99;
    else if (strlen(s1) < strlen(s2))
        return -99;
    else
    {
        if (strcmp(s1, s2) > 0)
            return 99;
        else if (strcmp(s1, s2) < 0)
            return -99;
        else
            return 0;
    }
}

heap *initHeap()
{
    heap *h = (heap *)malloc(sizeof(heap));

    h->size = 0;
    h->capacity = LEN;
    return h;
}

void percolateDownMin(heap *h, int index)
{

    int min;

    if ((2 * index + 1) <= h->size)
    {
        if (compareTo(h->stringArray[2 * index], h->stringArray[2 * index + 1]) > 0)
            min = 2 * index + 1;
        else
            min = 2 * index;

        if (compareTo(h->stringArray[index], h->stringArray[min]) > 0)
        {
            swap(h, index, min);
            percolateDownMin(h, min);
        }
    }
    else if (h->size == 2 * index)
    {
        if (compareTo(h->stringArray[index], h->stringArray[2 * index]) > 0)
            swap(h, index, 2 * index);
    }
}

void percolateUpMin(heap *h, int index)
{
    if (index > 1)
    {
        if (compareTo(h->stringArray[index / 2], h->stringArray[index]) > 0)
        {
            swap(h, index, index / 2);
            percolateUpMin(h, index / 2);
        }
    }
}

void percolateDownMax(heap *h, int index)
{
    int max;

    if ((2 * index + 1) <= h->size)
    {
        if (compareTo(h->stringArray[2 * index], h->stringArray[2 * index + 1]) > 0)
            max = 2 * index;
        else
            max = 2 * index+1;

        if (compareTo(h->stringArray[index], h->stringArray[max]) < 0)
        {
            swap(h, index, max);
            percolateDownMax(h, max);
        }
    }
    else if (h->size == 2 * index)
    {
        if (compareTo(h->stringArray[index], h->stringArray[2 * index]) < 0)
            swap(h, index, 2 * index);
    }
}

void percolateUpMax(heap *h, int index)
{
    if (index > 1)
    {
        if (compareTo(h->stringArray[index / 2], h->stringArray[index]) < 0)
        {
            swap(h, index, index / 2);
            percolateUpMax(h, index / 2);
        }
    }
}

int insertForMin(heap *h, char value[])
{
    strcpy(h->stringArray[++h->size], value);
    percolateUpMin(h, h->size);
}

int insertForMax(heap *h, char value[])
{
    strcpy(h->stringArray[++h->size], value);
    percolateUpMax(h, h->size);
}

char *removeMin(heap *h)
{
    if (h->size > 0)
    {   
        char *retval = malloc(sizeof(char) * (1 + strlen(h->stringArray[1])));
        strcpy(retval, h->stringArray[1]);
        strcpy(h->stringArray[1], h->stringArray[h->size]);

        h->size--;

        percolateDownMin(h, 1);

        return retval;
    }
    else
        return NULL;
}


char *removeMax(heap *h)
{
    if (h->size > 0)
    {
        char *retval = malloc(sizeof(char) * (1 + strlen(h->stringArray[1])));
        strcpy(retval, h->stringArray[1]);
        strcpy(h->stringArray[1], h->stringArray[h->size]);

        h->size--;

        percolateDownMax(h, 1);

        return retval;
    }
    else
        return NULL;
}


void printheap(heap *h)
{
    for (int i = 1; i <= h->size; i++)
        printf("%s ", h->stringArray[i]);
    printf("\n");
}


void swap(heap *h, int index1, int index2)
{
    char temp[LEN*4];
    strcpy(temp, h->stringArray[index1]);
    strcpy(h->stringArray[index1], h->stringArray[index2]);
    strcpy(h->stringArray[index2], temp);
}

char* peek(heap* h)
{
    return h->stringArray[1];
}

void freeheap(heap *h)
{
    free(h);
}
