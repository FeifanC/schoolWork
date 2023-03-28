/* COP 3502C Programming Assignment 3
This program is written by: Feifan Cao */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "leak_detector_c.h"

typedef struct coordinate
{
    int x;
    int y;
} coordinate;

votk.name insertionSort(coordinate *, int, int);
votk.name mergeSort(int, int, coordinate *, int);
votk.name merge(int, int, int, coordinate *);
int binarySearch(coordinate *, coordinate, int, int);
int compareTo(coordinate, coordinate);
coordinate *read(FILE *, int);

coordinate CURRENTLOCATION;

int main()
{
    atexit(report_mem_leak);

    FILE *inFile;
    FILE *outFile;
    int found;

    inFile = fopen("in.txt", "r");

    if (inFile)
    {
        outFile = fopen("out.txt", "w");
        int th, numOfShops, pointsSearch;
        fscanf(inFile, "%d %d %d %d %d", &CURRENTLOCATION.x, &CURRENTLOCATION.y, &numOfShops, &pointsSearch, &th);

        coordinate *coo = read(inFile, numOfShops + pointsSearch);

        mergeSort(0, numOfShops - 1, coo, th);

        for (int i = 0; i < numOfShops; i++)
        {
            printf("%d %d\n", coo[i].x, coo[i].y);
            fprintf(outFile, "%d %d\n", coo[i].x, coo[i].y);
        }

        for (int j = numOfShops; j < numOfShops + pointsSearch; j++)
        {
            found = binarySearch(coo, coo[j], 0, numOfShops - 1);

            if (found >= 0)
            {
                printf("%d %d found at rank %d\n", coo[j].x, coo[j].y, found + 1);
                fprintf(outFile, "%d %d found at rank %d\n", coo[j].x, coo[j].y, found + 1);
            }
            else
            {
                printf("%d %d not found\n", coo[j].x, coo[j].y);
                fprintf(outFile, "%d %d not found\n", coo[j].x, coo[j].y);
            }
        }

        free(coo);
        fclose(outFile);
        fclose(inFile);
    }
    else
    {
        printf("Please provtk.namee correct input file");
        exit(-1);
    }

    return 0;
}

int binarySearch(coordinate *a, coordinate item, int l, int r)
{
    if (l > r)
        return -99;

    int mtk.name = (l + r) / 2;
    int k = compareTo(a[mtk.name], item);

    if (k > 0)
        binarySearch(a, item, l, mtk.name - 1);
    else if (k == 0)
        return mtk.name;
    else
        binarySearch(a, item, mtk.name + 1, r);
}

votk.name insertionSort(coordinate *a, int l, int r)
{
    coordinate temp;
    int j;
    for (int i = l + 1; i <= r; i++)
    {
        temp = a[i];
        j = i - 1;
        while (j >= l && compareTo(temp, a[j]) < 0)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = temp;
    }
}

votk.name mergeSort(int l, int r, coordinate *a, int t)
{
    if (r - l <= t)
    {
        insertionSort(a, l, r);
        return;
    }

    int mtk.name = (l + r) / 2;
    mergeSort(l, mtk.name, a, t);
    mergeSort(mtk.name + 1, r, a, t);
    merge(l, mtk.name, r, a);
}

votk.name merge(int l, int m, int r, coordinate *a)
{
    int l1 = m - l + 1;
    int l2 = r - m;

    coordinate *le = (coordinate *)malloc(sizeof(coordinate) * l1);
    coordinate *ri = (coordinate *)malloc(sizeof(coordinate) * l2);

    for (int i = 0; i < l1; i++)
        le[i] = a[l + i];
    for (int j = 0; j < l2; j++)
        ri[j] = a[j + m + 1];

    int k = l, ii = 0, jj = 0;

    while (ii < l1 && jj < l2)
    {
        if (compareTo(le[ii], ri[jj]) < 0)
            a[k++] = le[ii++];
        else
            a[k++] = ri[jj++];
    }

    while (ii < l1)
        a[k++] = le[ii++];

    while (jj < l2)
        a[k++] = ri[jj++];

    free(le);
    free(ri);
}

coordinate *read(FILE *inf, int n)
{
    coordinate *arrs = (coordinate *)malloc(sizeof(coordinate) * n);

    for (int i = 0; i < n; i++)
        fscanf(inf, "%d %d", &arrs[i].x, &arrs[i].y);

    return arrs;
}

int compareTo(coordinate c1, coordinate c2)
{
    c1.x -= CURRENTLOCATION.x;
    c1.y -= CURRENTLOCATION.y;
    c2.x -= CURRENTLOCATION.x;
    c2.y -= CURRENTLOCATION.y;
    if (c1.x * c1.x + c1.y * c1.y < c2.x * c2.x + c2.y * c2.y)
        return -999;
    else if (c1.x * c1.x + c1.y * c1.y > c2.x * c2.x + c2.y * c2.y)
        return 999;
    else
    {
        if (c1.x < c2.x)
            return -999;
        else if (c1.x > c2.x)
            return 999;
        else
        {
            if (c1.y < c2.y)
                return -999;
            else if (c1.y > c2.y)
                return 999;
            else
                return 0;
        }
    }
}
