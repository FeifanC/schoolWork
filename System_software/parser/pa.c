/*
    Feifan Cao, Juan M Garcia
    Dr. Montagne
    COP3402 Spring
    HW3 - Tiny PL/0 compile
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fseek(inf, -1, SEEK_CUR);

#define NORW 15      /* number of reserved words */
#define IMAX 32767   /* maximum integer value */
#define CMAX 20      /* maximum number of chars for tk.name */
#define STRMAX 256   /* maximum length of strings */
#define JUSTBIG 1000 /*just a big number for the arrays size*/
#define ERROR 999    /*error code*/
#define MAX_SYMBOL_TABLE_SIZE 500
#define CODE_SIZE 500

typedef struct
{
    int kind;      // const = 1, var = 2
    char name[12]; // name up to 11 chars
    int val;       // number
    int level;     // L level
    int addr;      // M address
    int mark;      // To indicate unavailable or deleted
} symbol;

typedef struct // output struct for printing op codes
{
    char op[4];
    int r;
    int m;
    int l;
} text;

typedef struct
{
    char *name;
} token;

typedef struct
{
    int m;
} code;

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
text tex[JUSTBIG];
token tk;
code codes[JUSTBIG];

int si = 0;        // symbol table index
int lineCount = 0; // line count for the input program
int numVars = -1;
int m = 3;
int ti = 0; // index for op lines
int l = 0;
int ci = 0;
char cal_symbol[JUSTBIG];
int calin = 0;
int condFlag = 1;

void print_symb_table();
int symb_table_check(char *str);
int DFS_specialCh(char letter);
int check_for_reserved(char *word);
int DFS_id(char *c);
int DFS_num(char *c);
char *getNextToken(FILE *inf);
int check_mulSymbol(char *c); // checking :=
void error_f(int list_num);
int const_decla_f(FILE *inf);
int expression_f(FILE *inf);
int factor_f(FILE *inf);
int condition_f(FILE *inf);
int statement_f(FILE *inf);
int term_f(FILE *inf);
int var_decla_f(FILE *inf);
int block_f(FILE *inf);
void print_Emit();
void emit(char op[4], int l, int m); // function used to send different op codes to array
void calculateArr();

int main(int argc, char **argv)
{
    FILE *inf;

    inf = fopen(argv[1], "r");
    if (inf == NULL)
    {
        printf("Error: <opening file>");
        exit(1);
    }
    else
    {

        if (block_f(inf) != -1)
        {
            if (strcmp(tk.name, ".") != 0)
            {
                error_f(1);
            }
            print_Emit();
        }

        fclose(inf);
    }
}

void print_Emit()
{
    printf("Assembly Code: \n");
    printf("Line    OP    L    M\n");
    printf("  0    JMP    0    3\n");
    for (int i = 0; i < ti; i++)
    {
        if (strcmp(tex[i].op, "call") == 0 || strcmp(tex[i].op, "precedure") == 0 || strcmp(tex[i].op, "else") == 0)
        {
            printf("9 ");
        }

        printf("  %d    %s    %d    %d\n", lineCount++, tex[i].op, tex[i].l, tex[i].m);
    }

    printf("  %d    SYS    %d    %d\n", lineCount, l, m);

    printf("\nSymbol Table:\n");
    printf("Kind | Name        | Value | Level | Address | Mark\n");
    printf("---------------------------------------------------\n");
    printf("   3 |        main |     0 |     0 |     3 |     1\n");
    print_symb_table();
}

void print_symb_table()
{
    for (int i = 0; i < si; i++)
    {
        printf("   %d |           %s |     %d |     %d |     %d |     1\n", symbol_table[i].kind, symbol_table[i].name, symbol_table[i].val, symbol_table[i].level, symbol_table[i].addr);
    }
}

int symb_table_check(char *str)
{
    for (int i = 0; i < si; i++)
    {
        if (strcmp(str, symbol_table[i].name) == 0)
            return i;
    }
    return -1;
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

int check_for_reserved(char *word)
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

int DFS_id(char *c)
{
    if (isdigit(c[0])) // if first char of the identifier is digit then it's error
    {
        return -1;
    }

    if (strlen(c) > 11)
    {
        // identifier can't be over 11 chars
        return -1;
    }

    for (int i = 0; i < strlen(c); i++)
    {
        if (DFS_specialCh(c[i]))
        {
            return -1;
        }
    }
    return 2;
}

int DFS_num(char *c)
{
    if (atoi(c) > IMAX)
    {
        return -1;
    }

    for (int i = 0; i < strlen(c); i++)
    {

        if (!isdigit(c[i]))
        {
            return -1;
        }
    }
    return 3;
}

char *getNextToken(FILE *inf)
{
    char c;
    char *cur_read_str = malloc(sizeof(char) * CMAX);
    int i = 0;

    if ((c = fgetc(inf)) == ' ' || (c == '\n') || (c == '\t'))
        while ((c = fgetc(inf)) == ' ' || (c == '\n') || (c == '\t'))
            ;

    if (c == ':' && ((c = fgetc(inf)) == '='))
        return ":=";

    if (DFS_specialCh(c))
    {
        cur_read_str[0] = c;
        cur_read_str[1] = '\0';
        return cur_read_str;
    }

    while (!DFS_specialCh(c) && (c != '\n') && (c != ' '))
    {
        cur_read_str[i++] = c;
        c = fgetc(inf);
    }

    cur_read_str[i] = '\0';
    fseek(inf, -1, SEEK_CUR);

    return cur_read_str;
}

int check_mulSymbol(char *c)
{
    if (strcmp(c, ":=") == 0)
        return 20;
    return -1;
}

void error_f(int list_num)
{ // finish the error message follow the instruction
    printf("Error: <");
    if (list_num == 1)
    {
        printf("Program must end with period>\n");
        exit(1);
    }
    else if (list_num == 2)
    {
        printf("const, var, and read keywords must be followed by identifier>\n");
        exit(1);
    }
    else if (list_num == 3)
    {
        printf("symbol name has already been declared>\n");
        exit(1);
    }
    else if (list_num == 4)
    {
        printf("constants must be assigned with =>\n");
        exit(1);
    }
    else if (list_num == 5)
    {
        printf("constants must be assigned an integer value>\n");
        exit(1);
    }
    else if (list_num == 6)
    {
        printf("constant and variable declarations must be followed by a semicolon>\n");
        exit(1);
    }
    else if (list_num == 7)
    {
        printf("undeclared identifier>\n");
        exit(1);
    }
    else if (list_num == 8)
    {
        printf("only variable values may be altered>\n");
        exit(1);
    }
    else if (list_num == 9)
    {
        printf("assignment statements must use :=>\n");
        exit(1);
    }
    else if (list_num == 10)
    {
        printf("begin must be followed by end>\n");
        exit(1);
    }
    else if (list_num == 11)
    {
        printf("if must be followed by then>\n");
        exit(1);
    }
    else if (list_num == 12)
    {
        printf("while must be followed by do>\n");
        exit(1);
    }
    else if (list_num == 13)
    {
        printf("condition must contain comparison operator>\n");
        exit(1);
    }
    else if (list_num == 14)
    {
        printf("right parenthesis must follow left parenthesis>\n");
        exit(1);
    }
    else if (list_num == 15)
    {
        printf("arithmetic equations must contain operands, parentheses, numbers, or symbols>\n");
        exit(1);
    }
    else
    {
        // printf("No error found");
    }
}

void emit(char op[4], int l, int m)
{

    // printf(" %s \n",op);
    strcpy(tex[ti].op, op); // opcode
    tex[ti].r = 0;          // register    not used in your project
    tex[ti].l = l;          // lexicographical level
    tex[ti].m = m;          // modifier
    ti++;
}

int const_decla_f(FILE *inf)
{
    do
    {
        tk.name = getNextToken(inf);

        if (DFS_id(tk.name) < 0)
        {
            error_f(2);
            return -1;
        }

        if (symb_table_check(tk.name) != -1)
        {
            error_f(3);
            return -1;
        }

        symbol_table[si].kind = 1;
        strcpy(symbol_table[si].name, tk.name);

        tk.name = getNextToken(inf);

        if (DFS_specialCh(tk.name[0]) != 9)
        {
            error_f(4);
            return -1;
        }

        tk.name = getNextToken(inf);

        if (DFS_num(tk.name) > 0)
        {
            symbol_table[si].val = atoi(tk.name);
            symbol_table[si].addr = 0;
            symbol_table[si].level = 0;
            si++;
        }
        else
        {
            error_f(5);
            return -1;
        }

        tk.name = getNextToken(inf);

    } while (strcmp(tk.name, ",") == 0);

    if (strcmp(tk.name, ";") != 0)
    {
        error_f(6);
        return -1;
    }

    tk.name = getNextToken(inf);

    return 1;
}

int expression_f(FILE *inf)
{
    int cur_val = 0;

    if (strcmp(tk.name, "-") == 0)
    {

        tk.name = getNextToken(inf);

        cur_val = term_f(inf);

        if (cur_val == -1)
            return -1;

        emit("NEG", l, m);

        while (strcmp(tk.name, "+") == 0 || strcmp(tk.name, "-") == 0)
        {
            if (strcmp(tk.name, "+") == 0)
            {
                tk.name = getNextToken(inf);

                int cur_val2 = term_f(inf);

                if (cur_val2 == -1)
                    return -1;

                cur_val = cur_val2 + cur_val;

                emit("ADD", l, m);
            }
            else
            {
                tk.name = getNextToken(inf);

                int cur_val2 = term_f(inf);

                if (cur_val2 == -1)
                    return -1;

                cur_val = cur_val - cur_val2;

                emit("SUB", l, m);
            }
        }
    }
    else
    {
        if (strcmp(tk.name, "+") == 0)
        {
            tk.name = getNextToken(inf);
        }

        cur_val = term_f(inf);

        if (cur_val == -1)
            return -1;

        while (strcmp(tk.name, "+") == 0 || strcmp(tk.name, "-") == 0)
        {
            if (strcmp(tk.name, "+") == 0)
            {
                tk.name = getNextToken(inf);

                int cur_val2 = term_f(inf);

                if (cur_val2 == -1)
                    return -1;

                cur_val = cur_val + cur_val2;

                emit("ADD", l, m);
            }
            else
            {
                tk.name = getNextToken(inf);

                int cur_val2 = term_f(inf);

                if (cur_val2 == -1)
                    return -1;

                cur_val = cur_val - cur_val2;

                emit("SUB", l, m);
            }
        }
    }
    return cur_val;
}

int factor_f(FILE *inf)
{
    int cur_val = 1;

    if ((DFS_id(tk.name) > 0) && (DFS_num(tk.name) < 0))
    {

        int curTokenIndex = symb_table_check(tk.name);

        if (curTokenIndex == -1)
        {
            error_f(7);
            return -1;
        }

        if (symbol_table[curTokenIndex].kind == 1)
        {
            m = symbol_table[curTokenIndex].val;
            cur_val = m;
            emit("LIT", l, m);
        }
        else if (symbol_table[curTokenIndex].kind == 2)
        {
            m = symbol_table[curTokenIndex].addr;
            cur_val = symbol_table[curTokenIndex].val;
            emit("LOD", l, m);
        }
        else
        {
        }

        tk.name = getNextToken(inf);
    }
    else if (DFS_num(tk.name) > 0)
    {
        m = atoi(tk.name);

        emit("LIT", l, m);

        tk.name = getNextToken(inf);

        cur_val = m;
    }
    else if (strcmp(tk.name, "(") == 0)
    {

        tk.name = getNextToken(inf);

        int cur_val = expression_f(inf);

        if (strcmp(tk.name, ")") != 0)
        {
            error_f(14);
            return -1;
        }

        tk.name = getNextToken(inf);

        return cur_val;
    }
    else
    {
        error_f(15);
        return -1;
    }

    return cur_val;
}

int condition_f(FILE *inf)
{
    int cur_val = 0;

    if (strcmp(tk.name, "odd") == 0)
    {
        tk.name = getNextToken(inf);

        cur_val = expression_f(inf);

        if (cur_val == -1)
            return -1;

        if (cur_val % 2 != 1)
            cur_val = 0;

        emit("ODD", l, m);
    }
    else
    {
        cur_val = expression_f(inf);

        if (cur_val == -1)
            return -1;

        if (strcmp(tk.name, "=") == 0)
        {
            tk.name = getNextToken(inf);

            int cur_val2 = expression_f(inf);

            if (cur_val2 == -1)
                return -1;

            if (cur_val != cur_val2)
                cur_val = 0;

            emit("EQL", l, m);
        }
        else if (strcmp(tk.name, "!=") == 0)
        {
            tk.name = getNextToken(inf);

            int cur_val2 = expression_f(inf);

            if (cur_val2 == -1)
                return -1;

            if (cur_val2 == cur_val)
                cur_val = 0;

            emit("NEQ", l, m);
        }
        else if (strcmp(tk.name, "<") == 0)
        {
            tk.name = getNextToken(inf);

            int cur_val2 = expression_f(inf);

            if (cur_val >= cur_val2)
                cur_val = 0;

            emit("LSS", l, m);
        }
        else if (strcmp(tk.name, "<=") == 0)
        {
            tk.name = getNextToken(inf);

            int cur_val2 = expression_f(inf);

            if (cur_val2 == -1)
                return -1;

            if (cur_val > cur_val2)
                cur_val = 0;

            emit("LEQ", l, m);
        }
        else if (strcmp(tk.name, ">") == 0)
        {
            tk.name = getNextToken(inf);

            int cur_val2 = expression_f(inf);

            if (cur_val <= cur_val2)
                cur_val = 0;

            emit("GTR", l, m);
        }
        else if (strcmp(tk.name, ">=") == 0)
        {
            tk.name = getNextToken(inf);

            int cur_val2 = expression_f(inf);

            if (cur_val < cur_val2)
                cur_val = 0;

            emit("GEQ", l, m);
        }
        else
        {
            error_f(13);
            return -1;
        }
    }

    return cur_val;
}

int statement_f(FILE *inf)
{

    if ((!check_for_reserved(tk.name)) && DFS_id(tk.name))
    {
        int curTokenIndex = symb_table_check(tk.name);

        if (curTokenIndex == -1)
        {
            error_f(7);
            return -1;
        }

        if (symbol_table[curTokenIndex].kind != 2)
        {
            error_f(8);
            return -1;
        }

        tk.name = getNextToken(inf);

        if (strcmp(tk.name, ":=") != 0)
        {
            error_f(9);
            return -1;
        }

        tk.name = getNextToken(inf);

        if (condFlag == 1) // if the condition is true then run the statement
            symbol_table[curTokenIndex].val = expression_f(inf);
        else
            expression_f(inf);

        lineCount++;
        m = symbol_table[curTokenIndex].addr;
        emit("STO", l, m);
        return 1;
    }

    if (strcmp(tk.name, "begin") == 0)
    {
        do
        {

            tk.name = getNextToken(inf);

            int cur_val = statement_f(inf);

            if (cur_val == -1)
                return -1;

        } while (strcmp(tk.name, ";") == 0);

        if (strcmp(tk.name, "end") != 0)
        {
            error_f(10);
            return -1;
        }

        tk.name = getNextToken(inf);
        return 1;
    }
    if (strcmp(tk.name, "if") == 0)
    {
        tk.name = getNextToken(inf);

        int cur_val = condition_f(inf);

        condFlag = cur_val;

        if (cur_val == -1)
            return -1;

        int jpcIndex = ci;
        emit("JPC", l, m);

        if (strcmp(tk.name, "then") != 0)
        {
            error_f(11);
            return -1;
        }

        tk.name = getNextToken(inf);

        int cur_val2 = statement_f(inf);

        if (cur_val2 == -1)
            return -1;

        codes[jpcIndex].m = ci;

        condFlag = 1; // current condition is reset
        return 1;
    }
    if (strcmp(tk.name, "while") == 0)
    {
        tk.name = getNextToken(inf);
        int loopIndex = ci;

        int cur_val = condition_f(inf);

        if (cur_val == -1)
            return -1;

        if (strcmp(tk.name, "do") != 0)
        {
            error_f(12);
            return -1;
        }

        tk.name = getNextToken(inf);

        int jpcIdx = ci;

        emit("JPC", l, m);

        int cur_val2 = statement_f(inf);

        if (cur_val2 == -1)
            return -1;

        m = jpcIdx;
        emit("JMP", l, m);
        codes[jpcIdx].m = ci;

        return 1;
    }
    if (strcmp(tk.name, "read") == 0)
    {

        tk.name = getNextToken(inf);

        if (DFS_id(tk.name) < 0)
        {
            error_f(2);
            return -1;
        }

        int symIdx = symb_table_check(tk.name);

        if (symIdx == -1)
        {
            error_f(7);
            return -1;
        }
        if (symbol_table[symIdx].kind != 2)
        {
            error_f(8);
            return -1;
        }

        tk.name = getNextToken(inf);

        emit("READ", l, m);

        m = symbol_table[symIdx].addr;

        emit("STO", l, m);
        return 1;
    }

    if (strcmp(tk.name, "write") == 0)
    {
        tk.name = getNextToken(inf);

        int cur_val2 = expression_f(inf);

        if (cur_val2 == -1)
            return -1;

        emit("WRITE", l, m);

        return 1;
    }

    ci++;
    return 1;
}

int term_f(FILE *inf)
{

    int cur_val = factor_f(inf);

    if (cur_val == -1)
        return -1;

    while (strcmp(tk.name, "*") == 0 || strcmp(tk.name, "/") == 0)
    {
        if (strcmp(tk.name, "*") == 0)
        {
            tk.name = getNextToken(inf);

            int cur_val2 = factor_f(inf);

            if (cur_val2 == -1)
                return -1;

            cur_val = cur_val * cur_val2;

            m += 1;
            emit("MUL", l, m);

            return cur_val;
        }
        else
        {
            tk.name = getNextToken(inf);

            int cur_val2 = factor_f(inf);

            if (cur_val2 == -1)
                return -1;

            cur_val = cur_val / cur_val2;

            m += 1;
            emit("DIV", l, m);

            return cur_val;
        }
    }
    return cur_val;
}

int var_decla_f(FILE *inf)
{
    int numVars = 0;
    int i = 0;
    char c;
    int addCount = 0;

    do
    {
        tk.name = getNextToken(inf);

        if (DFS_id(tk.name) < 0)
        {
            error_f(2);
            return -1;
        }
        if (symb_table_check(tk.name) != -1)
        {
            error_f(3);
            return -1;
        }

        symbol_table[si].kind = 2;
        strcpy(symbol_table[si].name, tk.name);
        symbol_table[si].level = 0;
        symbol_table[si].val = 0;
        symbol_table[si].addr = 3;
        symbol_table[si].addr += si;
        si++;

        numVars++;
        tk.name = getNextToken(inf);

    } while (DFS_specialCh(tk.name[0]) == 17);

    if (DFS_specialCh(tk.name[0]) != 18)
    {
        error_f(6);
        return -1;
    }

    tk.name = getNextToken(inf);

    return numVars;
}

int block_f(FILE *inf)
{
    tk.name = getNextToken(inf);

    if (strcmp(tk.name, "const") == 0)
    {
        if (const_decla_f(inf) < 0)
            return -1;
    }
    else if (strcmp(tk.name, "var") == 0)
    {
        numVars = var_decla_f(inf);

        if (numVars < 0)
            return -1;

        emit("INC", 0, 3 + numVars);
    }
    else
    {
    }

    int cur_val = statement_f(inf);

    if (cur_val == -1) return -1;

    return 1;
}