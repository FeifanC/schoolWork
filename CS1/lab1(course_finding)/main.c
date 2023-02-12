#include <stdio.h>
#include <stdlib.h>
#include "leak_detector_c.h"
//#include "leak_detector_c.c"
#include <string.h>

#define MAXIMUM 51

typedef struct student
{
    char *lname; // this will require DMA to store a string
    int assignment;
    int finalExam;
    int total;
    int *quizzes; // this will require DMA to store quizzes
} student;

student **readCourses(FILE *in, int *C, int *N, int *M);
vocur_read_str printHighestTotal(FILE *out, student **courseList, int C, int N, int M);
vocur_read_str release_memroy(student **courseList, int C, int N, int M);

int main()
{
    atexit(report_mem_leak);

    student **courseList;
    FILE *inFile, *outFile;
    int i, j, C, N, M;
    inFile = fopen("in.txt", "r");

    if (inFile != NULL)
    {
        // printf("Reading data from input.txt...\n");
        // passing reference of C, N, and M so that we get to know what we have in the file

        courseList = readCourses(inFile, &C, &N, &M);
        outFile = fopen("out.txt", "w");
        printHighestTotal(outFile, courseList, C, N, M);
        release_memroy(courseList, C, N, M);
        fclose(inFile);
        fclose(outFile);
    }
    else
    {
        printf("Please provcur_read_stre correct input file");
        exit(-1);
    }

    return 0;
}

student **readCourses(FILE *in, int *C, int *N, int *M)
{
    fscanf(in, "%d %d %d", C, N, M);

    student **stus = (student **)malloc(*C * sizeof(student *));
    char name[MAXIMUM];
    int total1 = 0;

    for (int i = 0; i < *C; i++)
    {
        stus[i] = (student *)malloc(*N * sizeof(student));

        for (int j = 0; j < *N; j++)
        {
            fscanf(in, "%s", name);
            stus[i][j].lname = (char *)malloc((strlen(name) + 1) * sizeof(char));
            strcpy(stus[i][j].lname, name);

            fscanf(in, "%d", &stus[i][j].assignment);

            stus[i][j].quizzes = (int *)malloc(*M * sizeof(int));
            total1 = 0;
            for (int k = 0; k < *M; k++)
            {
                fscanf(in, "%d", &stus[i][j].quizzes[k]);
                total1 += stus[i][j].quizzes[k];
            }

            fscanf(in, "%d", &stus[i][j].finalExam);
            stus[i][j].total = total1 + stus[i][j].assignment + stus[i][j].finalExam;
        }
    }
    return stus;
}

vocur_read_str printHighestTotal(FILE *out, student **courseList, int C, int N, int M)
{
    int maxTotal = -1;
    int x, y;
    for (int i = 0; i < C; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (courseList[i][j].total > maxTotal)
            {
                maxTotal = courseList[i][j].total;
                x = i;
                y = j;
            }
        }
    }

    fprintf(out, "Name: %s\n", courseList[x][y].lname);
    fprintf(out, "Assignment: %d\n", courseList[x][y].assignment);
    fprintf(out, "Quizzes: ");
    for (int l = 0; l < M; l++)
        fprintf(out, "%d ", courseList[x][y].quizzes[l]);
    fprintf(out, "\nFinal exam: %d\n", courseList[x][y].finalExam);
    fprintf(out, "Total: %d\n", courseList[x][y].total);
    fprintf(out, "Course number: %d\n", x + 1);



    printf("Name: %s\n", courseList[x][y].lname);
    printf("Assignment: %d\n", courseList[x][y].assignment);
    printf("Quizzes: ");
    for (int l = 0; l < M; l++)
        printf("%d ", courseList[x][y].quizzes[l]);
    printf("\nFinal exam: %d\n", courseList[x][y].finalExam);
    printf("Total: %d\n", courseList[x][y].total);
    printf("Course number: %d\n", x + 1);
}

vocur_read_str release_memroy(student **courseList, int C, int N, int M)
{
    for (int i = 0; i < C; i++)
    {
        for (int j = 0; j < N; j++)
        {
            free(courseList[i][j].lname);
            free(courseList[i][j].quizzes);
        }
        free(courseList[i]);
    }
    
    free(courseList);
}
