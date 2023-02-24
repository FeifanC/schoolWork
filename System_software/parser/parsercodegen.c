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
#define CMAX 20      /* maximum number of chars for cur_read_str */
#define STRMAX 256   /* maximum length of strings */
#define JUSTBIG 1000 /*just a big number for the arrays size*/
#define ERROR 999    /*error code*/
#define MAX_SYMBOL_TABLE_SIZE 500

typedef struct
{
    int kind;      // const = 1, var = 2
    char name[12]; // name up to 11 chars
    int val;       // number
    int level;     // L level
    int addr;      // M address
} symbol;

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

int si = 0; // symbol table index

int symb_table_check(char *str)
{
    for (int i = 0; i < MAX_SYMBOL_TABLE_SIZE; i++)
    {
        if (strcmp(str, symbol_table[i].name) == 0)
            return i;
    }
    return -1;
}

int check_for_reserved(char* word)
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
// Function to check the identifiers making sure they are valid
int DFS_id(char *c)
{
    if (isdigit(c[0])) // if first char of the identifier is digit then it's error
        return -1;
    if (strlen(c) > 11)
    {
        // identifier can't be over 11 chars
        printf("Invalid identifier\n");
        return -1;
    }

    for (int i = 0; i < strlen(c); i++)
    {
        if (!isalpha(c[i]))
            return -1;
    }
    return 2;
}
// Checking the number making sure is valid.
int DFS_num(char *c)
{
    for (int i = 0; i < strlen(c); i++)
    {

        if (!isdigit(c[i]))
            return -1;
    }

    if (atoi(c) > IMAX)
    {
        // if the integer value bigger than 32767
        printf("Invalid integer\n");
        return -1;
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

void error_f(int list_num)
{ // finish the error message follow the instruction
    if (list_num == 1)
    {
        printf("Program must end with period\n");
    }
    else if (list_num == 2)
    {
        printf("const, var, and read keywords must be followed by identifier");
    }
    else if (list_num == 3)
    {
        printf("symbol name has already been declare");
    }
    else if (list_num == 4)
    {
        printf("constants must be assigned with =");
    }
    else if (list_num == 5)
    {
        printf("constants must be assigned an integer value");
    }
    else if (list_num == 6)
    {
        printf("constant and variable declarations must be followed by a semicolon");
    }
    else if (list_num == 7)
    {
        printf("undeclared identifier");
    }
    else if (list_num == 8)
    {
        printf("only variable values may be altered");
    }
    else if (list_num == 9)
    {
        printf("assignment statements must use :=");
    }
    else if (list_num == 10)
    {
        printf("begin must be followed by end");
    }
    else if (list_num == 11)
    {
        printf("if must be followed by then");
    }
    else if (list_num == 12)
    {
        printf("while must be followed by do");
    }
    else if (list_num == 13)
    {
        printf("condition must contain comparison operator");
    }
    else if (list_num == 14)
    {
        printf("right parenthesis must follow left parenthesis");
    }
    else if (list_num == 15)
    {
        printf("arithmetic equations must contain operands, parentheses, numbers, or symbols");
    }
    else
    {
    }
}

int const_decla_f(FILE *inf)
{
    char c;
    int i = 0;
    char* cur_read_str = malloc(sizeof(char) * CMAX);

    do
    {
        while (!DFS_specialCh((c = fgetc(inf))) && (c != '\n'))
        {
            if(c == ' ') continue;
            cur_read_str[i++] = c;
        }

        cur_read_str[i] = '\0';

        if (DFS_id(cur_read_str) < 0)
        {
            error_f(2);
            return -1;
        }
        if (symb_table_check(cur_read_str) != -1)
        {
            error_f(3);
            return -1;
        }

        symbol_table[si].kind = 1;
        strcpy(symbol_table[si].name, cur_read_str);

        if (DFS_specialCh(c) != 9)
        {
            error_f(4);
            return -1;
        }

        if((c = fgetc(inf)) == ' ') c = fgetc(inf);

        i = 0;
        if (isdigit(c))
        {
            cur_read_str[i++] = c;
            while (((c = fgetc(inf)) != ' ') && (!DFS_specialCh(c)) && (c != '\n'))
            {
                cur_read_str[i++] = c;
            }

            cur_read_str[i] = '\0';
            i = 0;

            if (DFS_num(cur_read_str) < 0)
            {
                error_f(5);
                return -1;
            }

            symbol_table[si].val = atoi(cur_read_str);
            symbol_table[si].addr = 0;
            symbol_table[si].level = 0;
            si++;
        }

    } while (c == ',');

    if (c != ';')
    {
        error_f(6);
        return -1;
    }

    free(cur_read_str);
}

void block_f(FILE *inf)
{
    char *cur_read_str = malloc(CMAX * sizeof(char));           // cur_read_strentifier
    char c;
    int i =0;

    while ((c = fgetc(inf)) != EOF)
    {
        // Check if next character is a letter or a number.
        if (isalpha(c))
        {
            cur_read_str[i++] = c;
            while ((c = fgetc(inf)) != ' ' && (!DFS_specialCh(c) && (c != '\n')))
            {
                cur_read_str[i++] = c;
            }
            cur_read_str[i] = '\0';
            i = 0;

            printf("%s\n", cur_read_str);
            if (check_for_reserved(cur_read_str) == 28)
            {
                if(const_decla_f(inf) < 0) return;
                //done
            }

            //plz work on below////////////////////

            break;
        }
        /////////////////////////////////////////////////////////////////////above is the const declaration

        // else if (c == ':')
        // {
        //     cur_read_str[i++] = c;
        //     c = fgetc(inf);
        //     cur_read_str[i++] = c;
        //     cur_read_str[i] = '\0';

        //     printf("%s      %d\n", cur_read_str, check_mulSymbol(cur_read_str));
        //     final_list_num[k++] = check_mulSymbol(cur_read_str);
        //     i = 0;
        // }
        // // Check weather is a digit if it is check the statement.
        // else if (isdigit(c))
        // {
        //     cur_read_str[i++] = c;
        //     while (((c = fgetc(inf)) != ' ') && (!DFS_specialCh(c)) && (c != '\n'))
        //     {
        //         cur_read_str[i++] = c;
        //     }

        //     cur_read_str[i] = '\0';
        //     i = 0;

        //     printf("%s      %d\n", cur_read_str, DFS_num(cur_read_str));
        //     final_list_num[k++] = DFS_num(cur_read_str);

        //     if (DFS_specialCh(c))
        //     {
        //         printf("%c      %d\n", c, DFS_specialCh(c));
        //         final_list_num[k++] = DFS_specialCh(c);
        //     }

        //     list_var_nums[p++] = atoi(cur_read_str);
        // }
        // else
        // {
        //     if (c == ' ' || c == '\n' || c == '\t' || c == '\b')
        //         continue;

        //     if (c == '\\' || c == '/') // checking comment symbol
        //     {
        //         cur_read_str[i++] = c;
        //         cur_read_str[i++] = fgetc(inf);
        //         cur_read_str[i] = '\0';
        //         if ((strcmp(cur_read_str, "\\*") == 0) || (strcmp(cur_read_str, "*/") == 0) || (strcmp(cur_read_str, "\\\\") == 0))
        //         {
        //             i = 0;
        //             continue;
        //         }
        //     }

        //     printf("%c      %d\n", c, DFS_specialCh(c));
        //     final_list_num[k++] = DFS_specialCh(c);
        // }
    }


    free(cur_read_str);
    fclose(inf);
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

        //char *token = malloc(CMAX * sizeof(char));
        printf("Kind | Name        | Value | Level | Address | Mark");
        printf("---------------------------------------------------");
        printf("   3 |        main |     0 |     0 |     3   |  0");
        block_f(inf);

        // if (strcmp(token, ".") != 0)
        // {
        //     error_f(1);
        //     printf("HALT");
        // }

        //free(token);
        fclose(inf);
    }
}

