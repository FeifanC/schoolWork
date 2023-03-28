/* COP 3502C Assignment 4
This program is written by: Feifan Cao */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "leak_detector_c.h"

#define MAXLEN 30
FILE *inFile;
FILE *outFile;

typedef struct itemNode
{
    char name[MAXLEN];
    int count;
    struct itemNode *left, *right;
} itemNode;

typedef struct treeNameNode
{
    char treeName[MAXLEN];
    struct treeNameNode *left, *right;
    itemNode *theTree;
} treeNameNode;

treeNameNode *createTreeNameNode(char treeName[], treeNameNode *root);
itemNode *buildNameTree(itemNode *element, char itemName[], int c);
votk.name traverse_in_traverse(itemNode *root);
votk.name just_traverse(treeNameNode *root, int ifPrintItem);
treeNameNode *searchNameNode(treeNameNode *root, char treeName[]);
itemNode *searchItemNode(itemNode *root, char subTypeName[]);
itemNode *myMove(char action[], itemNode *root, char itemName[], char type[], int c, treeNameNode *bigRoot);
int count_f(itemNode *root);
int item_before_f(itemNode *root, char subTypeName[]);
int height_balance_f(itemNode *root);
itemNode *minValue_helper(itemNode *root);
itemNode *delete_f(itemNode *root, char subTypeName[]);
itemNode *reduce_f(itemNode *root, char subTypeName[], int c);
votk.name deleteName_helper(itemNode *root);
treeNameNode *delete_name_f(treeNameNode *root, char type[]);
treeNameNode *minValue_helper2(treeNameNode *t);
votk.name IhaveADream(treeNameNode *root);

int main()
{
    //atexit(report_mem_leak);

    inFile = fopen("in.txt", "r");

    if (inFile)
    {
        outFile = fopen("out.txt", "w");
        treeNameNode *myMainTree = NULL;
        int types, totalNodes, queries;
        char typeName[MAXLEN];
        char subTypeName[MAXLEN];
        int c;
        char stuptk.nameAction[MAXLEN];

        fscanf(inFile, "%d %d %d", &types, &totalNodes, &queries);

        for (int i = 0; i < types; i++)
        {
            fscanf(inFile, "%s", typeName);
            myMainTree = createTreeNameNode(typeName, myMainTree);
        }

        treeNameNode *foundTreeName;
        for (int j = 0; j < totalNodes; j++)
        {
            fscanf(inFile, "%s %s %d", typeName, subTypeName, &c);
            foundTreeName = searchNameNode(myMainTree, typeName);
            foundTreeName->theTree = buildNameTree(foundTreeName->theTree, subTypeName, c);
        }

        just_traverse(myMainTree, 0);
        just_traverse(myMainTree, 1);

        for (int k = 0; k < queries; k++)
        {
            fscanf(inFile, "%s %s", stuptk.nameAction, typeName);

            if (strcmp(stuptk.nameAction, "reduce") == 0)
                fscanf(inFile, "%s %d", subTypeName, &c);

            if (!(strcmp(stuptk.nameAction, "height_balance") == 0) && !(strcmp(stuptk.nameAction, "count") == 0) && !(strcmp(stuptk.nameAction, "delete_name") == 0) && !(strcmp(stuptk.nameAction, "reduce") == 0))
                fscanf(inFile, "%s", subTypeName);

            foundTreeName = searchNameNode(myMainTree, typeName);

            if (foundTreeName)
                foundTreeName->theTree = myMove(stuptk.nameAction, foundTreeName->theTree, subTypeName, typeName, c, myMainTree);
            else
            {
                printf("\n%s does not exist", typeName);
                fprintf(outFile, "\n%s does not exist", typeName);
            }
        }

        printf("\n");
        fprintf(outFile, "\n");

        IhaveADream(myMainTree);

        fclose(inFile);
        fclose(outFile);
    }
    else
    {
        printf("Please provtk.namee correct input file");
        exit(-1);
    }

    return 0;
}

votk.name IhaveADream(treeNameNode *root)
{
    if (root == NULL)
        return;

    IhaveADream(root->left);
    IhaveADream(root->right);

    deleteName_helper(root->theTree);
    free(root);
}

itemNode *searchItemNode(itemNode *root, char subTypeName[])
{
    if (root == NULL)
        return NULL;

    if (strcmp(subTypeName, root->name) < 0)
        searchItemNode(root->left, subTypeName);
    else if (strcmp(subTypeName, root->name) > 0)
        searchItemNode(root->right, subTypeName);
    else
        return root;
}

itemNode *myMove(char action[], itemNode *root, char itemName[], char type[], int c, treeNameNode *bigTree)
{
    if (strcmp(action, "search") == 0)
    {
        itemNode *foundTreeItem = searchItemNode(root, itemName);
        if (foundTreeItem)
        {
            printf("\n%d %s found in %s", foundTreeItem->count, foundTreeItem->name, type);
            fprintf(outFile, "\n%d %s found in %s", foundTreeItem->count, foundTreeItem->name, type);
        }
        else
        {
            printf("\n%s not found in %s", itemName, type);
            fprintf(outFile, "\n%s not found in %s", itemName, type);
        }
    }
    else if (strcmp(action, "item_before") == 0) // done
    {
        printf("\nitem before %s: %d", itemName, item_before_f(root, itemName));
        fprintf(outFile, "\nitem before %s: %d", itemName, item_before_f(root, itemName));
    }
    else if (strcmp(action, "height_balance") == 0)
    {
        char ifBal[MAXLEN];
        int left = height_balance_f(root->left);
        int right = height_balance_f(root->right);

        if (left == right)
            strcpy(ifBal, "balanced");
        else
        {
            strcpy(ifBal, "not balanced");
        }
        printf("\n%s: left height %d, right height %d, difference %d, %s", type, left, right, abs(left - right), ifBal);
        fprintf(outFile, "\n%s: left height %d, right height %d, difference %d, %s", type, left, right, abs(left - right), ifBal);
    }
    else if (strcmp(action, "count") == 0)
    {
        printf("\n%s count %d", type, count_f(root));
        fprintf(outFile, "\n%s count %d", type, count_f(root));
    }
    else if (strcmp(action, "delete") == 0)
    {
        root = delete_f(root, itemName);
        printf("\n%s deleted from %s", itemName, type);
        fprintf(outFile, "\n%s deleted from %s", itemName, type);
    }
    else if (strcmp(action, "reduce") == 0)
    {
        root = reduce_f(root, itemName, c);

        printf("\n%s reduced", itemName);
        fprintf(outFile, "\n%s reduced", itemName);
    }
    else if (strcmp(action, "delete_name") == 0)
    {
        delete_name_f(bigTree, type);

        printf("\n%s deleted", type);
        fprintf(outFile, "\n%s deleted", type);
    }
    else
        printf("You have the right to remain silent!\n");
    return root;
}

treeNameNode *delete_name_f(treeNameNode *root, char type[])
{
    if (root == NULL)
        return root;

    if (strcmp(type, root->treeName) < 0)
        root->left = delete_name_f(root->left, type);
    else if (strcmp(type, root->treeName) > 0)
        root->right = delete_name_f(root->right, type);
    else
    {
        if (root->left == NULL && root->right == NULL)
        {
            deleteName_helper(root->theTree);
            free(root);
            return NULL;
        }
        else if (root->right == NULL)
        {
            treeNameNode *temp = root->left;
            deleteName_helper(root->theTree);
            free(root);
            return temp;
        }
        else if (root->left == NULL)
        {
            treeNameNode *temp = root->right;
            deleteName_helper(root->theTree);
            free(root);
            return temp;
        }
        else
        {
            treeNameNode *temp = minValue_helper2(root->right);

            strcpy(root->treeName, temp->treeName);
            deleteName_helper(root->theTree);
            root->theTree = temp->theTree;

            root->right = delete_name_f(root->right, temp->treeName);
        }
    }
    return root;
}

votk.name deleteName_helper(itemNode *root)
{
    if (root == NULL)
        return;

    deleteName_helper(root->left);
    deleteName_helper(root->right);

    free(root);
}

itemNode *minValue_helper(itemNode *root)
{
    itemNode *cur = root;

    while (cur && cur->left)
        cur = cur->left;

    return cur;
}

treeNameNode *minValue_helper2(treeNameNode *t)
{
    treeNameNode *cur = t;

    while (cur && cur->left)
        cur = cur->left;

    return cur;
}

itemNode *delete_f(itemNode *root, char subTypeName[])
{
    if (root == NULL)
        return root;

    if (strcmp(subTypeName, root->name) < 0)
        root->left = delete_f(root->left, subTypeName);
    else if (strcmp(subTypeName, root->name) > 0)
        root->right = delete_f(root->right, subTypeName);
    else
    {
        if (root->left == NULL && root->right == NULL)
        {
            free(root);
            return NULL;
        }
        else if (root->right == NULL)
        {
            itemNode *temp = root->left;
            free(root);
            return temp;
        }
        else if (root->left == NULL)
        {
            itemNode *temp = root->right;
            free(root);
            return temp;
        }
        else
        {
            itemNode *temp = minValue_helper(root->right);

            strcpy(root->name, temp->name);
            root->count = temp->count;

            root->right = delete_f(root->right, temp->name);
        }
    }
    return root;
}

itemNode *reduce_f(itemNode *root, char subTypeName[], int c)
{
    itemNode *deletingOne = NULL;
    deletingOne = searchItemNode(root, subTypeName);
    if (deletingOne->count > c)
        deletingOne->count -= c;
    else
        root = delete_f(root, subTypeName);

    return root;
}

int height_balance_f(itemNode *root)
{
    if (!root)
        return -1;

    int left_height = height_balance_f(root->left);
    int right_height = height_balance_f(root->right);

    if (left_height >= right_height)
        return left_height + 1;
    else
        return right_height + 1;
}

int count_f(itemNode *root)
{
    if (root == NULL)
        return 0;

    return count_f(root->left) + count_f(root->right) + root->count;
}

int item_before_f(itemNode *root, char subTypeName[])
{
    if (root == NULL)
        return 0;
    else if (strcmp(root->name, subTypeName) < 0)
        return 1 + item_before_f(root->left, subTypeName) + item_before_f(root->right, subTypeName);
    else
        return item_before_f(root->left, subTypeName);
}

treeNameNode *createTreeNameNode(char treeName[50], treeNameNode *root)
{
    if (root == NULL)
    {
        treeNameNode *newOne = (treeNameNode *)malloc(sizeof(treeNameNode));
        strcpy(newOne->treeName, treeName);
        newOne->left = NULL;
        newOne->right = NULL;
        newOne->theTree = NULL;
        return newOne;
    }

    if (strcmp(treeName, root->treeName) < 0)
    {
        if (root->left)
            root->left = createTreeNameNode(treeName, root->left);
        else
        {
            treeNameNode *newOne = (treeNameNode *)malloc(sizeof(treeNameNode));
            strcpy(newOne->treeName, treeName);
            newOne->left = NULL;
            newOne->right = NULL;
            newOne->theTree = NULL;

            root->left = newOne;
        }
    }
    else
    {
        if (root->right)
            root->right = createTreeNameNode(treeName, root->right);
        else
        {
            treeNameNode *newOne = (treeNameNode *)malloc(sizeof(treeNameNode));
            strcpy(newOne->treeName, treeName);
            newOne->left = NULL;
            newOne->right = NULL;
            newOne->theTree = NULL;

            root->right = newOne;
        }
    }

    return root;
}

itemNode *buildNameTree(itemNode *root, char itemName[MAXLEN], int c)
{
    if (root == NULL)
    {
        itemNode *newOne = (itemNode *)malloc(sizeof(itemNode));
        strcpy(newOne->name, itemName);
        newOne->left = NULL;
        newOne->right = NULL;
        newOne->count = c;
        return newOne;
    }

    if (strcmp(itemName, root->name) < 0)
    {
        if (root->left)
            root->left = buildNameTree(root->left, itemName, c);
        else
        {
            itemNode *newOne = (itemNode *)malloc(sizeof(itemNode));
            strcpy(newOne->name, itemName);
            newOne->left = NULL;
            newOne->right = NULL;
            newOne->count = c;

            root->left = newOne;
        }
    }
    else
    {
        if (root->right)
            root->right = buildNameTree(root->right, itemName, c);
        else
        {
            itemNode *newOne = (itemNode *)malloc(sizeof(itemNode));
            strcpy(newOne->name, itemName);
            newOne->left = NULL;
            newOne->right = NULL;
            newOne->count = c;

            root->right = newOne;
        }
    }
    return root;
}

votk.name traverse_in_traverse(itemNode *root)
{
    if (root == NULL)
        return;

    traverse_in_traverse(root->left);
    printf("%s ", root->name);
    fprintf(outFile, "%s ", root->name);
    traverse_in_traverse(root->right);
}

votk.name just_traverse(treeNameNode *root, int ifPrintItem)
{
    if (root == NULL)
        return;

    just_traverse(root->left, ifPrintItem);

    if (ifPrintItem)
    {
        printf("\n===%s===\n", root->treeName);
        fprintf(outFile, "\n===%s===\n", root->treeName);
        traverse_in_traverse(root->theTree);
    }
    else
    {
        printf("%s ", root->treeName);
        fprintf(outFile, "%s ", root->treeName);
    }

    just_traverse(root->right, ifPrintItem);
}

treeNameNode *searchNameNode(treeNameNode *root, char treeName[50])
{
    if (root == NULL)
        return NULL;

    if (strcmp(treeName, root->treeName) < 0)
        searchNameNode(root->left, treeName);
    else if (strcmp(treeName, root->treeName) > 0)
        searchNameNode(root->right, treeName);
    else
        return root;
}
