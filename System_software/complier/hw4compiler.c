/*
    Feifan Cao
    Dr. Montagne
    COP3402 Spring
    HW4 - NEW Tiny PL/0 compiler with lex reader
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// cx = ti
// fseek(inf, -1, SEEK_CUR);

#define NORW 15      /* number of reserved words */
#define IMAX 32767   /* maximum integer value */
#define CMAX 20      /* maximum number of chars for tk.name */
#define STRMAX 256   /* maximum length of strings */
#define JUSTBIG 1000 /*just a big number for the arrays size*/
#define ERROR 999    /*error code*/
#define MAX_SYMBOL_TABLE_SIZE 500
#define CODE_SIZE 500
#define ERRORCODE -1115111

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
    char op[6];
    int m;
    int l;
} text;

typedef struct
{
    char *name;
} token;

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
text tex[JUSTBIG]; // aka code
token tk;          // current token

int si = -1; // symbol table index
int m = 3;   // aka tx
int ti = -1; // index for op lines aka cx
int l = 0;
int jpm = 3;
int commentFlag = 0;
int condFlag = 1;
char fileName[JUSTBIG];
FILE *outFile;

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
int print_assemblyCode();
void print_src();
int proc_decla_f(FILE *inf);
void enter(int kind, char *name, int val, int level, int addr);
void mark();
void emit(char op[4], int l, int m); // function used to send different op codes to array

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
        strcpy(fileName, argv[1]);
        if (block_f(inf) != ERRORCODE)
        {
            if (strcmp(tk.name, ".") != 0)
            {
                error_f(1);
            }

            emit("SYS", l , m);

            print_src();
            print_assemblyCode();
        }
        fclose(inf);
    }
}

void mark()
{
    for (int i = si; i >= 0; i--)
    {
        if (symbol_table[i].mark == 1)
            continue;
        if (symbol_table[i].level < l)
            return;
        symbol_table[i].mark = 1;
    }
}

int print_assemblyCode()
{
    outFile = fopen("elf.txt", "w");

    printf("Assembly Code: \n");
    for (int a = 0; a <= ti; a++)
    {
        if (strcmp(tex[a].op, "LIT") == 0)
        {
            fprintf(outFile, "1  0  %d", tex[a].m);
            printf("LIT  0  %d", tex[a].m);
        }
        else if (strcmp(tex[a].op, "ADD") == 0)
        {
            fprintf(outFile, "2  0  1");
            printf("ADD  0  1");
        }
        else if (strcmp(tex[a].op, "SUB") == 0)
        {
            fprintf(outFile, "2  0  2");
            printf("SUB  0  2");
        }
        else if (strcmp(tex[a].op, "MUL") == 0)
        {
            fprintf(outFile, "2  0  3");
            printf("MUL  0  3");
        }
        else if (strcmp(tex[a].op, "DIV") == 0)
        {
            fprintf(outFile, "2  0  4");
            printf("DIV  0  4");
        }
        else if (strcmp(tex[a].op, "EQL") == 0)
        {
            fprintf(outFile, "2  0  5");
            printf("EQL  0  5");
        }

        else if (strcmp(tex[a].op, "NEQ") == 0)
        {
            fprintf(outFile, "2  0  6");
            printf("NEQ  0  6");
        }

        else if (strcmp(tex[a].op, "LSS") == 0)
        {
            fprintf(outFile, "2  0  7");
            printf("LSS  0  7");
        }

        else if (strcmp(tex[a].op, "LEQ") == 0)
        {
            fprintf(outFile, "2  0  8");
            printf("LEQ  0  8");
        }

        else if (strcmp(tex[a].op, "GTR") == 0)
        {
            fprintf(outFile, "2  0  9");
            printf("GTR  0  9");
        }
        else if (strcmp(tex[a].op, "GEQ") == 0)
        {
            fprintf(outFile, "2  0  10");
            printf("GEQ  0  10");
        }
        else if (strcmp(tex[a].op, "ODD") == 0)
        {
            fprintf(outFile, "2  0  11");
            printf("ODD  0  11");
        }
        else if (strcmp(tex[a].op, "LOD") == 0)
        {
            fprintf(outFile, "3  %d  %d", tex[a].l, tex[a].m);
            printf("LOD  %d  %d", tex[a].l, tex[a].m);
        }
        else if (strcmp(tex[a].op, "STO") == 0)
        {
            fprintf(outFile, "4  %d  %d", tex[a].l, tex[a].m);
            printf("STO  %d  %d", tex[a].l, tex[a].m);
        }
        else if (strcmp(tex[a].op, "CAL") == 0)
        {
            fprintf(outFile, "5  %d  %d", tex[a].l, tex[a].m);
            printf("CAL  %d  %d", tex[a].l, tex[a].m);
        }
        else if (strcmp(tex[a].op, "INC") == 0)
        {
            fprintf(outFile, "6  0  %d", tex[a].m);
            printf("INC  0  %d", tex[a].m);
        }
        else if (strcmp(tex[a].op, "JMP") == 0)
        {
            fprintf(outFile, "7  0  %d", tex[a].m);
            printf("JMP  0  %d", tex[a].m);
        }
        else if (strcmp(tex[a].op, "JPC") == 0)

        {
            fprintf(outFile, "8  0  %d", tex[a].m);
            printf("JPC  0  %d", tex[a].m);
        }
        else if (strcmp(tex[a].op, "SYS") == 0)
        {
            fprintf(outFile, "9  0  3");
            printf("SYS  0  3");
        }
        else if (strcmp(tex[a].op, "READ") == 0)
        {
            fprintf(outFile, "9  0  2");
            printf("READ  0  2");
        }
        else
        {
            fprintf(outFile, "9  0  1");
            printf("WRITE  0  1");
        }

        printf("\n");
        fprintf(outFile, "\n");
    }
}

void print_src()
{
    FILE *inf = fopen(fileName, "r");
    char c;

    while ((c = fgetc(inf)) != EOF)
    {
        printf("%c", c);
    }

    printf("\nNo errors, program is syntactically correct\n\n");
}

void print_Emit()
{
    int lineCount = 0;

    printf("Assembly Code: \n");
    printf("Line    OP    L    M\n");
    for (int i = 0; i <= ti; i++)
    {
        if (strcmp(tex[i].op, "call") == 0 || strcmp(tex[i].op, "precedure") == 0 || strcmp(tex[i].op, "else") == 0)
        {
            printf("9 ");
        }

        printf("  %d    %s    %d    %d\n", lineCount++, tex[i].op, tex[i].l, tex[i].m);
    }

    printf("  %d    SYS    %d    %d\n", lineCount, l, m);
}

int symb_table_check(char *str)
{
    int curIndex = -1;
    int maxLevel = -1;

    for (int i = 0; i <= si; i++)
    {
        if (strcmp(str, symbol_table[i].name) == 0 && (symbol_table[i].level == l))
            return i;
        if (strcmp(str, symbol_table[i].name) == 0 && symbol_table[i].level > maxLevel)
        {
            curIndex = i;
            maxLevel = symbol_table[i].level;
        }
    }

    return curIndex;
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
    {

        return ":=";
    }

    if (c == '/' && ((c = fgetc(inf)) == '/'))
    {
        commentFlag = 1;
        while ((c = fgetc(inf)) != '\n')
            ;
    }

    if (commentFlag == 1)
    {
        commentFlag = 0;
        fseek(inf, -1, SEEK_CUR);
    }

    if (DFS_specialCh(c))
    {
        cur_read_str[0] = c;
        cur_read_str[1] = '\0';
        return cur_read_str;
    }

    while (!DFS_specialCh(c) && (c != '\n') && (c != ' ') && (c != ':'))
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
{
    FILE *inf = fopen(fileName, "r");
    char c;

    while ((c = fgetc(inf)) != EOF)
    {
        printf("%c", c);
    }

    printf("\nError: Error number ");
    if (list_num == 1)
    {
        printf("9: Period expected\n");
        exit(1);
    }
    else if (list_num == 2)
    {
        printf("4: const, var, Procedure keywords must be followed by identifier\n");
        exit(1);
    }
    else if (list_num == 3)
    {
        printf("3: symbol name has already been declared\n");
        exit(1);
    }
    else if (list_num == 4)
    {
        printf("5: constants must be assigned with =\n");
        exit(1);
    }
    else if (list_num == 5)
    {
        printf("2: constants must be assigned an integer value\n");
        exit(1);
    }
    else if (list_num == 6)
    {
        printf("10: constant and variable declarations must be followed by a semicolon\n");
        exit(1);
    }
    else if (list_num == 7)
    {
        printf("11: undeclared identifier\n");
        exit(1);
    }
    else if (list_num == 8)
    {
        printf("8: only variable values may be altered>\n");
        exit(1);
    }
    else if (list_num == 9)
    {
        printf("1: Use = instead of := \n");
        exit(1);
    }
    else if (list_num == 10)
    {
        printf("6: begin must be followed by end\n");
        exit(1);
    }
    else if (list_num == 11)
    {
        printf("7: if must be followed by then\n");
        exit(1);
    }
    else if (list_num == 12)
    {
        printf("12: while must be followed by do\n");
        exit(1);
    }
    else if (list_num == 13)
    {
        printf("13: condition must contain comparison operator\n");
        exit(1);
    }
    else if (list_num == 14)
    {
        printf("22: right parenthesis must follow left parenthesis>\n");
        exit(1);
    }
    else if (list_num == 15)
    {
        printf("16: arithmetic equations must contain operands, parentheses, numbers, or symbols>\n");
        exit(1);
    }
    else if (list_num == 16)
    {
        printf("17: Semicolon or end expected after procedure declaration\n");
    }
    else if (list_num == 17)
    {
        printf("14: Call must be followed by an identifier\n");
    }
    else if (list_num == 18)
    {
        printf("15:	Call of a constant or variable is meaningless.\n");
    }
    else
    {
        // printf("No error found");
    }
}

void emit(char op[4], int l, int m)
{
    ti++;
    // printf(" %s \n",op);
    strcpy(tex[ti].op, op); // opcode
    tex[ti].l = l;          // lexicographical level
    tex[ti].m = m;          // m
}

int const_decla_f(FILE *inf)
{
    do
    {
        tk.name = getNextToken(inf);

        if (DFS_id(tk.name) < 0)
        {
            error_f(2);
            return ERRORCODE;
        }

        if (symb_table_check(tk.name) != -1)
        {
            error_f(3);
            return ERRORCODE;
        }

        symbol_table[si].kind = 1;
        strcpy(symbol_table[si].name, tk.name);

        tk.name = getNextToken(inf);

        if (DFS_specialCh(tk.name[0]) != 9)
        {
            error_f(4);
            return ERRORCODE;
        }

        tk.name = getNextToken(inf);

        if (DFS_num(tk.name) > 0)
        {
            symbol_table[si].val = atoi(tk.name);
            symbol_table[si].addr = 0;
            symbol_table[si].level = 0;
            symbol_table[si].mark = 0;
            si++;

            mark();
        }
        else
        {
            error_f(5);
            return ERRORCODE;
        }

        tk.name = getNextToken(inf);

    } while (strcmp(tk.name, ",") == 0);

    if (strcmp(tk.name, ";") != 0)
    {
        error_f(6);
        return ERRORCODE;
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

        if (cur_val == ERRORCODE)
            return ERRORCODE;

        emit("NEG", l, 6);

        while (strcmp(tk.name, "+") == 0 || strcmp(tk.name, "-") == 0)
        {
            if (strcmp(tk.name, "+") == 0)
            {
                tk.name = getNextToken(inf);

                int cur_val2 = term_f(inf);

                if (cur_val2 == ERRORCODE)
                    return ERRORCODE;

                cur_val = cur_val2 + cur_val;

                emit("ADD", l, 1);
            }
            else
            {
                tk.name = getNextToken(inf);

                int cur_val2 = term_f(inf);

                if (cur_val2 == ERRORCODE)
                    return ERRORCODE;

                cur_val = cur_val - cur_val2;

                emit("SUB", l, 2);
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

        if (cur_val == ERRORCODE)
            return ERRORCODE;

        while (strcmp(tk.name, "+") == 0 || strcmp(tk.name, "-") == 0)
        {
            if (strcmp(tk.name, "+") == 0)
            {
                tk.name = getNextToken(inf);

                int cur_val2 = term_f(inf);

                if (cur_val2 == ERRORCODE)
                    return ERRORCODE;

                cur_val = cur_val + cur_val2;

                emit("ADD", l, 1);
            }
            else
            {
                tk.name = getNextToken(inf);

                int cur_val2 = term_f(inf);

                if (cur_val2 == ERRORCODE)
                    return ERRORCODE;

                cur_val = cur_val - cur_val2;

                emit("SUB", l, 2);
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
            return ERRORCODE;
        }

        if (symbol_table[curTokenIndex].kind == 1)
        {
            m = symbol_table[curTokenIndex].val;
            cur_val = m;
            emit("LIT", 0, m); // si is tp
        }
        else if (symbol_table[curTokenIndex].kind == 2)
        {
            m = symbol_table[curTokenIndex].addr;
            cur_val = symbol_table[curTokenIndex].val;
            emit("LOD", symbol_table[si].level, m); // si is tp
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
            return ERRORCODE;
        }

        tk.name = getNextToken(inf);

        return cur_val;
    }
    else
    {
        error_f(15);
        return ERRORCODE;
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

        if (cur_val == ERRORCODE)
            return ERRORCODE;

        if (cur_val % 2 != 1)
            cur_val = 0;

        emit("ODD", l, 11);
    }
    else
    {
        cur_val = expression_f(inf);

        if (cur_val == ERRORCODE)
            return ERRORCODE;

        if (strcmp(tk.name, "=") == 0)
        {

            tk.name = getNextToken(inf);

            int cur_val2 = expression_f(inf);

            if (cur_val2 == ERRORCODE)
                return ERRORCODE;

            if (cur_val != cur_val2)
                cur_val = 0;

            emit("EQL", l, 5);
        }
        else if (strcmp(tk.name, "!=") == 0)
        {
            tk.name = getNextToken(inf);

            int cur_val2 = expression_f(inf);

            if (cur_val2 == ERRORCODE)
                return ERRORCODE;

            if (cur_val2 == cur_val)
                cur_val = 0;

            emit("NEQ", l, 6);
        }
        else if (strcmp(tk.name, "<") == 0)
        {
            tk.name = getNextToken(inf);

            int cur_val2 = expression_f(inf);

            if (cur_val2 == ERRORCODE)
                return ERRORCODE;

            if (cur_val >= cur_val2)
                cur_val = 0;

            emit("LSS", l, m);
        }
        else if (strcmp(tk.name, "<=") == 0)
        {
            tk.name = getNextToken(inf);

            int cur_val2 = expression_f(inf);

            if (cur_val2 == ERRORCODE)
                return ERRORCODE;

            if (cur_val > cur_val2)
                cur_val = 0;

            emit("LEQ", l, m);
        }
        else if (strcmp(tk.name, ">") == 0)
        {
            tk.name = getNextToken(inf);

            int cur_val2 = expression_f(inf);

            if (cur_val2 == ERRORCODE)
                return ERRORCODE;

            if (cur_val <= cur_val2)
                cur_val = 0;

            emit("GTR", l, m);
        }
        else if (strcmp(tk.name, ">=") == 0)
        {
            tk.name = getNextToken(inf);

            int cur_val2 = expression_f(inf);

            if (cur_val2 == ERRORCODE)
                return ERRORCODE;

            if (cur_val < cur_val2)
                cur_val = 0;

            emit("GEQ", l, m);
        }
        else
        {
            error_f(13);
            return ERRORCODE;
        }
    }

    return cur_val;
}

int statement_f(FILE *inf)
{

    if ((!check_for_reserved(tk.name)) && DFS_id(tk.name) > 0)
    {
        int curTokenIndex = symb_table_check(tk.name);

        if (curTokenIndex == -1)
        {
            error_f(7);
            return ERRORCODE;
        }

        if (symbol_table[curTokenIndex].kind != 2)
        {
            error_f(8);
            return ERRORCODE;
        }

        tk.name = getNextToken(inf);

        if (strcmp(tk.name, ":=") != 0)
        {
            error_f(9);
            return ERRORCODE;
        }

        tk.name = getNextToken(inf);

        if (condFlag == 1) // if the condition is true then run the statement
            symbol_table[curTokenIndex].val = expression_f(inf);
        else
            expression_f(inf);

        m = symbol_table[curTokenIndex].addr;
        emit("STO", symbol_table[si].level, m);
        return 1;
    }
    else if (strcmp(tk.name, "call") == 0)
    {
        tk.name = getNextToken(inf);

        if (DFS_id(tk.name) < 0)
        {
            error_f(17);
            return ERRORCODE;
        }

        int procPosition = symb_table_check(tk.name);

        if (procPosition == -1)
        {
            error_f(7);
            return ERRORCODE;
        }

        if (symbol_table[procPosition].kind == 3)
        {
            emit("CAL", symbol_table[procPosition].level, symbol_table[procPosition].addr);
        }
        else
        {
            error_f(18);
            return ERRORCODE;
        }

        tk.name = getNextToken(inf);
    }

    if (strcmp(tk.name, "begin") == 0)
    {
        do
        {
            tk.name = getNextToken(inf);

            int cur_val = statement_f(inf);

            if (cur_val == ERRORCODE)
                return ERRORCODE;

        } while (strcmp(tk.name, ";") == 0);

        if (strcmp(tk.name, "end") != 0)
        {
            error_f(10);
            return ERRORCODE;
        }

        tk.name = getNextToken(inf);
        return 1;
    }
    if (strcmp(tk.name, "if") == 0)
    {
        tk.name = getNextToken(inf);

        int cur_val = condition_f(inf);

        condFlag = cur_val;

        if (cur_val == ERRORCODE)
            return ERRORCODE;

        int jpcIndex = ti;

        emit("JPC", 0, jpcIndex);

        if (strcmp(tk.name, "then") != 0)
        {
            error_f(11);
            return ERRORCODE;
        }

        tk.name = getNextToken(inf);

        int cur_val2 = statement_f(inf);

        if (cur_val2 == ERRORCODE)
            return ERRORCODE;

        tex[jpcIndex].m = ti;

        condFlag = 1; // current condition is reset
        return 1;
    }
    if (strcmp(tk.name, "while") == 0)
    {
        tk.name = getNextToken(inf);

        int loopIndex = ti * 3;

        int cur_val = condition_f(inf);

        if (cur_val == ERRORCODE)
            return ERRORCODE;

        if (strcmp(tk.name, "do") != 0)
        {
            error_f(12);
            return ERRORCODE;
        }

        tk.name = getNextToken(inf);

        int jpcIdx = ti;

        emit("JPC", 0, jpcIdx);

        int cur_val2 = statement_f(inf);

        if (cur_val2 == ERRORCODE)
            return ERRORCODE;

        m = loopIndex;

        emit("JMP", 0, loopIndex);
        tex[jpcIdx].m = ti;

        return 1;
    }
    if (strcmp(tk.name, "read") == 0)
    {

        tk.name = getNextToken(inf);

        if (DFS_id(tk.name) < 0)
        {
            error_f(2);
            return ERRORCODE;
        }

        int symIdx = symb_table_check(tk.name);

        if (symIdx == -1)
        {
            error_f(7);
            return ERRORCODE;
        }
        if (symbol_table[symIdx].kind != 2)
        {
            error_f(8);
            return ERRORCODE;
        }

        tk.name = getNextToken(inf);

        emit("READ", 0, 2);

        m = symbol_table[symIdx].addr;

        emit("STO", symbol_table[si].level, m);
        return 1;
    }

    if (strcmp(tk.name, "write") == 0)
    {
        tk.name = getNextToken(inf);

        int cur_val2 = expression_f(inf);

        if (cur_val2 == ERRORCODE)
            return -1;

        emit("WRITE", 0, 1);

        return 1;
    }

    return 1;
}

int term_f(FILE *inf)
{
    int cur_val = factor_f(inf);

    if (cur_val == ERRORCODE)
        return ERRORCODE;

    while (strcmp(tk.name, "*") == 0 || strcmp(tk.name, "/") == 0)
    {
        if (strcmp(tk.name, "*") == 0)
        {
            tk.name = getNextToken(inf);

            int cur_val2 = factor_f(inf);

            if (cur_val2 == ERRORCODE)
                return ERRORCODE;

            cur_val = cur_val * cur_val2;

            m += 1;
            emit("MUL", 0, 3);

            return cur_val;
        }
        else
        {
            tk.name = getNextToken(inf);

            int cur_val2 = factor_f(inf);

            if (cur_val2 == ERRORCODE)
                return ERRORCODE;

            cur_val = cur_val / cur_val2;

            m += 1;
            emit("DIV", 0, 4);

            return cur_val;
        }
    }
    return cur_val;
}

void enter(int k, char *n, int v, int l, int a)
{
    si++;
    symbol_table[si].kind = k;
    strcpy(symbol_table[si].name, n);
    symbol_table[si].level = l;
    symbol_table[si].val = v;
    symbol_table[si].addr = a + si;
    symbol_table[si].mark = 0;
}

int var_decla_f(FILE *inf)
{
    int numVars = 0;

    do
    {
        tk.name = getNextToken(inf);

        if (DFS_id(tk.name) < 0)
        {
            error_f(2);
            return ERRORCODE;
        }

        int varIndex = symb_table_check(tk.name);
        if ((varIndex != -1) && (symbol_table[varIndex].level) == l)
        {
            error_f(3);
            return ERRORCODE;
        }

        enter(2, tk.name, 0, l, m);
        mark();

        numVars++;
        tk.name = getNextToken(inf);

    } while (DFS_specialCh(tk.name[0]) == 17);

    if (DFS_specialCh(tk.name[0]) != 18)
    {
        error_f(6);
        return ERRORCODE;
    }

    tk.name = getNextToken(inf);

    return numVars;
}

int proc_decla_f(FILE *inf)
{
    tk.name = getNextToken(inf);

    if (DFS_id(tk.name) < 0)
    {
        error_f(2);
        return ERRORCODE;
    }

    enter(3, tk.name, 0, l, m);
    mark();

    tk.name = getNextToken(inf);

    if (strcmp(tk.name, ";") != 0)
    {
        error_f(16);
        return ERRORCODE;
    }

    return si;
}

int block_f(FILE *inf)
{
    emit("JMP", l, jpm);

    jpm += m * 3;
    int numVars = 0;

    tk.name = getNextToken(inf);

    do
    {
        if (strcmp(tk.name, "const") == 0)
        {
            if (const_decla_f(inf) < 0)
                return ERRORCODE;
        }
        else if (strcmp(tk.name, "var") == 0)
        {
            numVars = var_decla_f(inf);

            if (numVars < 0)
                return ERRORCODE;

            m += numVars;
        }
        else
        {
        }

        while (strcmp(tk.name, "procedure") == 0)
        {
            l++;

            int tx = proc_decla_f(inf);

            if (tx == ERRORCODE)
            {
                return ERRORCODE;
            }

            int numVarProc = block_f(inf);

            if (numVarProc != ERRORCODE)
            {
                if (strcmp(tk.name, ";") != 0)
                {
                    error_f(16);
                    return ERRORCODE;
                }

                m += numVarProc;

                symbol_table[tx].addr = m;

                tk.name = getNextToken(inf);
            }
            else
            {
                return ERRORCODE;
            }
        }
    } while (strcmp(tk.name, "const") == 0 || strcmp(tk.name, "var") == 0 || strcmp(tk.name, "procedure") == 0);

    emit("INC", 0, m);

    int cur_val = statement_f(inf);

    if (cur_val == ERRORCODE)
        return ERRORCODE;

    l--;
    return numVars;
}