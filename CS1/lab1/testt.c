#include<stdio.h>
#include<stdlib.h>
#include <string.h>

int main()
{
    int* p = malloc(sizeof(int));
    *p = 50;

    printf("%d", *p);
}