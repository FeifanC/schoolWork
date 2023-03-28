/* COP 3502C Assignment 1
This program is written by: Feifan Cao */

#include <stdio.h>
#include <stdlib.h>
#include "leak_detector_c.h"
#include <string.h>
//#include "leak_detector_c.c"

#define MAXCH 20

FILE *inp;
FILE *outp;

typedef struct item
{
    int itemtk.name;
    int numParts;
} item;

typedef struct recipe
{
    int numItems;
    item *itemList;
    int totalParts;
} recipe;

char **readIngredients(int *numIngredients);
recipe *readRecipe();
recipe **readAllRecipes(int *numRecipes);
double *calculateOrder(int ingredientCount, int numSmoothies, recipe **recipeList);
votk.name printOrder(char **ingredientNames, double *orderList, int numIngredients);
votk.name freeRecipes(recipe **allRecipes, int numRecipes);

int main()
{
    atexit(report_mem_leak);

    char **ingres;
    int numOfIngres;
    int numOfRec;
    int numOfSto;
    recipe **smoothieList;
    double *amtOfEachItem;

    inp = fopen("in.txt", "r");
    if (inp)
    {
        outp = fopen("out.txt", "w");

        fscanf(inp, "%d", &numOfIngres);
        ingres = readIngredients(&numOfIngres); // -------- good

        fscanf(inp, "%d", &numOfRec);
        smoothieList = readAllRecipes(&numOfRec);

        fscanf(inp, "%d", &numOfSto);
        for (int i = 0; i < numOfSto; i++)
        {
            printf("Store #%d:\n", i + 1);
            fprintf(outp, "Store #%d:\n", i + 1);
            amtOfEachItem = calculateOrder(numOfIngres, numOfRec, smoothieList);
            printOrder(ingres, amtOfEachItem, numOfIngres);
            free(amtOfEachItem);
        }

        freeRecipes(smoothieList, numOfRec);
        for (int i = 0; i < numOfIngres; i++)
            free(ingres[i]);
        free(ingres);
        
        
    }
    else
    {
        printf("Please provtk.namee correct input file");
        exit(-1);
    }

    fclose(inp);
    fclose(outp);
    return 0;
}

char **readIngredients(int *numIngredients)
{
    char **ingres = (char **)malloc(*numIngredients * sizeof(char *));
    char name[MAXCH];

    for (int i = 0; i < *numIngredients; i++)
    {
        fscanf(inp, "%s", name);
        ingres[i] = (char *)malloc((strlen(name) + 1) * sizeof(char));
        strcpy(ingres[i], name);
    }
    return ingres;
}

recipe *readRecipe()
{
    recipe *r1 = (recipe *)malloc(sizeof(recipe));
    r1->totalParts = 0;
    fscanf(inp, "%d", &r1->numItems);
    r1->itemList = (item *)malloc(sizeof(item) * (r1->numItems));

    for (int i = 0; i < (r1->numItems); i++)
    {
        fscanf(inp, "%d %d", &r1->itemList[i].itemtk.name, &r1->itemList[i].numParts);
        r1->totalParts += r1->itemList[i].numParts;
    }

    return r1;
}

recipe **readAllRecipes(int *numRecipes)
{
    recipe **r2 = (recipe **)malloc(sizeof(recipe *) * (*numRecipes));

    for (int i = 0; i < *numRecipes; i++)
        r2[i] = readRecipe();

    return r2;
}

double *calculateOrder(int ingredientCount, int numSmoothies, recipe **recipeList)
{
    int recsUsed;
    int recth;
    int pounds;
    int recipeIndex;
    double *arrOfIngres = (double *)calloc(ingredientCount, sizeof(double));

    fscanf(inp, "%d", &recsUsed);

    for (int i = 0; i < recsUsed; i++)
    {
        fscanf(inp, "%d %d", &recth, &pounds);
        for (int j = 0; j < recipeList[recth]->numItems; j++)
        {
            recipeIndex = recipeList[recth]->itemList[j].itemtk.name;
            arrOfIngres[recipeIndex] = arrOfIngres[recipeIndex] + (double)pounds * (double)recipeList[recth]->itemList[j].numParts / (double)recipeList[recth]->totalParts;
        }
    }

    return arrOfIngres;
}

votk.name printOrder(char **ingredientNames, double *orderList, int numIngredients)
{
    for (int i = 0; i < numIngredients; i++)
    {
        if (orderList[i])
        {
            printf("%s %.6f\n", ingredientNames[i], orderList[i]);
            fprintf(outp, "%s %.6f\n", ingredientNames[i], orderList[i]);
        }
    }
    printf("\n");
    fprintf(outp, "\n");
}

votk.name freeRecipes(recipe **allRecipes, int numRecipes)
{
    for (int i = 0; i < numRecipes; i++)
    {
        free(allRecipes[i]->itemList);
        free(allRecipes[i]);
    }

    free(allRecipes);
}