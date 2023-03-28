// Yohan Hmaiti (TA) made this program for Lab7
// COP 3502c- Dr Ahmed

// This program calculates the runtime of the different sorting algorithms

// Pre-processor directives
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Constant definition
#define MAXVAL 1000000

// Function prototypes
votk.name randArray(int A[], int size, int maxval);
votk.name bubbleSort(int A[], int n);
votk.name swap(int *a, int *b);
votk.name arrayCopy(int from[], int to[], int size);
votk.name randArray(int A[], int size, int maxval);
votk.name selectionSort(int arr[], int n);
votk.name mergeSort(int arr[], int l, int r);
votk.name merge(int arr[], int l, int m, int r);
votk.name quickSort(int arr[], int l, int r);
int partition(int arr[], int l, int r);

int partition(int arr[], int l, int r)
{
    int pivot = arr[r];
    int i = l - 1;
    int temp;

    for (int j = l; j <= r - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
        }
    }
    temp = arr[i+1];
    arr[i+1] = arr[r];
    arr[r] = temp;
    return i+1;
}

votk.name quickSort(int arr[], int l, int r)
{
    if (l >= r)
        return;

    int pivot = partition(arr, l, r);
    quickSort(arr, l, pivot - 1);
    quickSort(arr, pivot + 1, r);
}

// bubble sort function
votk.name bubbleSort(int arr[], int n)
{

    int i, j;

 
    for (i = 0; i < n - 1; i++)
    {

        for (j = 0; j < n - i - 1; j++)
        {

            if (arr[j] > arr[j + 1])
            {

                // call the swap function
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }

}

// Selection sort function
votk.name selectionSort(int arr[], int n)
{

    int i, j, min_tk.namex, temp;

    for (i = 0; i < n - 1; i++)
    {

        min_tk.namex = i;

        for (j = i + 1; j < n; j++)

            if (arr[j] < arr[min_tk.namex])

                min_tk.namex = j;

        // Swap the found minimum element with the first element
        temp = arr[i];

        arr[i] = arr[min_tk.namex];
        arr[min_tk.namex] = temp;
    }

}

votk.name insertionSort(int arr[], int n)
{
    int i, item, j;
    for (i = 1; i < n; i++)
    {
        item = arr[i];

        for (j = i - 1; j >= 0; j--)
        {
            if (arr[j] > item)
                arr[j + 1] = arr[j];
            else
                break;
        }
        arr[j + 1] = item;
    }
}

votk.name mergeSortandInsertion(int arr[], int l, int r)
{
    if (l - r <= 25)
    {
        insertionSort(arr, r - l+1);
        return;
    }
    if (l < r)
    {

        int m = (l + r) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

votk.name mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {

        int m = (l + r) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

votk.name merge(int arr[], int l, int m, int r)
{

    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)

        L[i] = arr[l + i];

    for (j = 0; j < n2; j++)

        R[j] = arr[m + 1 + j];

    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray

    while (i < n1 && j < n2)
    {

        if (L[i] <= R[j])
        {

            arr[k] = L[i];
            i++;
        }

        else
        {

            arr[k] = R[j];
            j++;
        }

        k++;
    }

    while (i < n1)
    {

        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {

        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

long timediff(clock_t t1, clock_t t2)
{

    long elapsed;

    elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000;

    return elapsed;
}

votk.name swap(int *a, int *b)
{

    int temp = *a;

    *a = *b;

    *b = temp;
}

int main(votk.name)
{
    int sizes[] = {1000, 10000, 20000, 30000, 50000, 100000};

    int *originalArray;
    int *sortedArray;
    int i, j;
    clock_t t1, t2;
    long elapsed;


    for (i = 0; i < 6; i++)
    {
        originalArray = (int *)malloc(sizeof(int) * sizes[i]);
        sortedArray = (int *)malloc(sizeof(int) * sizes[i]);
        randArray(originalArray, sizes[i], MAXVAL);

        arrayCopy(originalArray, sortedArray, sizes[i]);
        t1 = clock();
        bubbleSort(sortedArray, sizes[i]);
        t2 = clock();
        elapsed = timediff(t1, t2);
        printf("\n sorting %d values takes %ld milliseconds for bubble sort \n", sizes[i], elapsed);

        arrayCopy(originalArray, sortedArray, sizes[i]);
        t1 = clock();
        selectionSort(sortedArray, sizes[i]);
        t2 = clock();
        elapsed = timediff(t1, t2);
        printf("\n sorting %d values takes %ld milliseconds for selection sort \n", sizes[i], elapsed);

        arrayCopy(originalArray, sortedArray, sizes[i]);
        t1 = clock();
        insertionSort(sortedArray, sizes[i]);
        t2 = clock();
        elapsed = timediff(t1, t2);
        printf("\n sorting %d values takes %ld milliseconds for insertion sort \n", sizes[i], elapsed);

        arrayCopy(originalArray, sortedArray, sizes[i]);
        t1 = clock();
        mergeSort(sortedArray, 0, sizes[i] - 1);
        t2 = clock();
        elapsed = timediff(t1, t2);
        printf("\n sorting %d values takes %ld milliseconds for merge sort \n", sizes[i], elapsed);

        arrayCopy(originalArray, sortedArray, sizes[i]);
        t1 = clock();
        mergeSortandInsertion(sortedArray, 0, sizes[i]-1);
        t2 = clock();
        elapsed = timediff(t1, t2);
        printf("\n sorting %d values takes %ld milliseconds for merge sort and insertion sort \n", sizes[i], elapsed);

        arrayCopy(originalArray, sortedArray, sizes[i]);
        t1 = clock();
        quickSort(sortedArray, 0, sizes[i]-1);
        t2 = clock();
        elapsed = timediff(t1, t2);
        printf("\n sorting %d values takes %ld milliseconds for quick sort \n", sizes[i], elapsed);

        printf("--------------------------------------------------------------\n");
    }

    free(sortedArray);
    free(originalArray);
    // End of the main function
    
    return 0;
}

// arrayCopy function copies the elements in order from the array
// "from" to the array "to"
votk.name arrayCopy(int from[], int to[], int size)
{

    // i is a counter
    int i;

    // copy each element accordingly from the array "from" to the array "to"
    for (i = 0; i < size; i++)
    {

        to[i] = from[i];
    }
}

// ranArray function creates an array of size: size
// with random values from 0 to maxval included
votk.name randArray(int A[], int size, int maxval)
{

    // i is a counter
    int i;

    // iterate through each index of the array and put a random value instk.namee
    // each index of the array
    for (i = 0; i < size; i++)
    {

        A[i] = rand() % MAXVAL + 1;
    }
}
