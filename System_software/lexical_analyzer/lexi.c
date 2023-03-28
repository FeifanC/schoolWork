/*
    Feifan Cao, Juan M Garcia
    Dr. Montagne
    COP3402 Spring
    HW2 - Lexical Analyzer
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NORW 15      /* number of reserved words */
#define IMAX 32767   /* maximum integer value */
#define CMAX 20      /* maximum number of chars for tk.nameents */
#define STRMAX 256   /* maximum length of strings */
#define JUSTBIG 1000 /*just a big number for the arrays size*/
#define ERROR 999    /*error code*/

int check_for_reserved(char word[])
{
    if (strcmp(word, "call") == 0)
        return 27;
    else if (strcmp(word, "begin") == 0)
        return 21;
    else if (strcmp(word, "end") == 0)
        return 22;
    else if (strcmp(word, "if") == 0)
        return 23;
    else if (strcmp(word, "then") == 0)
        return 24;
    else if (strcmp(word, "else") == 0)
        return 33;
    else if (strcmp(word, "while") == 0)
        return 25;
    else if (strcmp(word, "read") == 0)
        return 32;
    else if (strcmp(word, "null") == 0)
        return 1;
    else if (strcmp(word, "const") == 0)
        return 28;
    else if (strcmp(word, "do") == 0)
        return 26;
    else if (strcmp(word, "odd") == 0)
        return 8;
    else if (strcmp(word, "procedure") == 0)
        return 30;
    else if (strcmp(word, "var") == 0)
        return 29;
    else if (strcmp(word, "write") == 0)
        return 31;
    else
        return 0;
}
//Function to check the identifiers making sure they are valid
int DFS_id(char *c)
{
    if (isdigit(c[0])) // if first char of the identifier is digit then it's error
        return 999;
    if (strlen(c) > 11){
        // identifier can't be over 11 chars
        printf("Invalid identifier\n");
        return 999;
    } 

    for (int i = 0; i < strlen(c); i++)
    {
        if (!isalpha(c[i]))
            return 999;
    }
    return 2;
}
//Checking the number making sure is valid.
int DFS_num(char *c)
{
    for (int i = 0; i < strlen(c); i++)
    {
        
        if (!isdigit(c[i]))
            return 999;
    }

    if (atoi(c) > IMAX){
        // if the integer value bigger than 32767
        printf("Invalid integer\n");
        return 999;
    }
    return 3;
}

int check_mulSymbol(char *c)
{
    if (strcmp(c, ":=") == 0)
        return 20;
    return 999;
}

int DFS_specialCh(char letter)
{
    if (letter == '+')
        return 4;
    else if (letter == '-')
        return 5;
    else if (letter == '*')
        return 6;
    else if (letter == '/')
        return 7;
    else if (letter == '(')
        return 15;
    else if (letter == ')')
        return 16;
    else if (letter == '=')
        return 9;
    else if (letter == ',')
        return 17;
    else if (letter == '.')
        return 19;
    else if (letter == '<')
        return 11;
    else if (letter == '>')
        return 13;
    else if (letter == ';')
        return 18;
    else if (letter == '#')
        return 10;
    else if (letter == '$')
        return 12;
    else if (letter == '%')
        return 14;
    else
        return 0;
}

/*main Program*/
int main(int argc, char **argv)
{
    FILE *inf;

    // opening File
    inf = fopen(argv[1], "r");
    if (inf == NULL)
    {
        printf("Error opening file!"); // checking file if it exists
        exit(1);
    }
    else
    {

        char *tk.name = malloc(CMAX * sizeof(char));           // tk.nameentifier
        int *final_list_num = (int *)malloc(JUSTBIG * sizeof(int)); // Final list of numbers
        char **list_var_name = malloc(JUSTBIG * sizeof(char));      // list of strings
        int *list_var_nums = malloc(JUSTBIG * sizeof(int));         // name of variables.
        char c;
        int i = 0;
        int k = 0; // index for number array
        int n = 0; // index for variable array
        int p = 0; // index for list of variable name

        printf("Source Program:\n");
        while ((c = fgetc(inf)) != EOF)
        {
            if (c == '\t')
                printf("  ");
            else
                printf("%c", c);
        }
        printf("\n\nLexeme Table:\n");
        printf("lexeme  token type\n");

        inf = fopen(argv[1], "r");
        while ((c = fgetc(inf)) != EOF)
        {
            //Check if next character is a letter or a number.
            if (isalpha(c))
            {
                tk.name[i++] = c;
                while ((c = fgetc(inf)) != ' ' && (!DFS_specialCh(c) && (c != '\n')))
                {
                    tk.name[i++] = c;
                }
                tk.name[i] = '\0';
                i = 0;

                if (check_for_reserved(tk.name))
                {
                    printf("%s      %d\n", tk.name, check_for_reserved(tk.name));
                    final_list_num[k++] = check_for_reserved(tk.name);
                }
                else
                {
                    printf("%s      %d\n", tk.name, DFS_id(tk.name));
                    final_list_num[k++] = DFS_id(tk.name);
                    // Allocation new array to keep track of the variables in order.
                    list_var_name[n] = malloc((strlen(tk.name) + 1) * sizeof(char));
                    strcpy(list_var_name[n], tk.name);

                    n++;
                }

                if (c == ' ' || c == '\n')
                    continue;

                printf("%c      %d\n", c, DFS_specialCh(c));
                final_list_num[k++] = DFS_specialCh(c);
            }
            else if (c == ':')
            {
                tk.name[i++] = c;
                c = fgetc(inf);
                tk.name[i++] = c;
                tk.name[i] = '\0';

                printf("%s      %d\n", tk.name, check_mulSymbol(tk.name));
                final_list_num[k++] = check_mulSymbol(tk.name);
                i = 0;
            }
            //Check weather is a digit if it is check the statement.
            else if (isdigit(c))
            {
                tk.name[i++] = c;
                while (((c = fgetc(inf)) != ' ') && (!DFS_specialCh(c)) && (c != '\n'))
                {
                    tk.name[i++] = c;
                }

                tk.name[i] = '\0';
                i = 0;

                printf("%s      %d\n", tk.name, DFS_num(tk.name));
                final_list_num[k++] = DFS_num(tk.name);

                if (DFS_specialCh(c))
                {
                    printf("%c      %d\n", c, DFS_specialCh(c));
                    final_list_num[k++] = DFS_specialCh(c);
                }

                list_var_nums[p++] = atoi(tk.name);
            }
            else
            {
                if (c == ' ' || c == '\n' || c == '\t' || c == '\b')
                    continue;

                if (c == '\\' || c == '/') // checking comment symbol
                {
                    tk.name[i++] = c;
                    tk.name[i++] = fgetc(inf);
                    tk.name[i] = '\0';
                    if ((strcmp(tk.name, "\\*") == 0) || (strcmp(tk.name, "*/") == 0) || (strcmp(tk.name, "\\\\") == 0))
                    {
                        i = 0;
                        continue;
                    }
                }

                printf("%c      %d\n", c, DFS_specialCh(c));
                final_list_num[k++] = DFS_specialCh(c);
            }
        }

        printf("\nLexeme List:\n");

        n = 0;
        p = 0;
        for (int i = 0; i < k; i++)
        {
            printf(" %d ", final_list_num[i]);
            if (final_list_num[i] == 2)
            {
                printf(" %s ", list_var_name[n]);
                n++;
            }
            if (final_list_num[i] == 3)
            {
                printf(" %d ", list_var_nums[p]);
                p++;
            }
        }

        free(tk.name);
        free(final_list_num);
        for (int z = 0; z < JUSTBIG; z++)
            free(list_var_nums);
        fclose(inf);
    }
}