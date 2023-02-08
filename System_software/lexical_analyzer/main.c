/*
    Feifan Cao, Juan M Garcia
    Dr. Montagne
    COP3402 Spring
    HW2 - Lexical Analyzer
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define norw 15    /* number of reserved words */
#define imax 32767 /* maximum integer value */
#define cmax 11    /* maximum number of chars for idents */
#define strmax 256 /* maximum length of strings */

typedef enum
{
    skipsym = 1,
    identsym,
    numbersym,
    plussym,
    minussym,
    multsym,
    slashsym,
    oddsym,
    eqsym,
    neqsym,
    lessym,
    leqsym,
    gtrsym,
    geqsym,
    lparentsym,
    rparentsym,
    commasym,
    semicolonsym,
    periodsym,
    becomessym,
    beginsym,
    endsym,
    ifsym,
    thensym,
    whilesym,
    dosym,
    callsym,
    constsym,
    varsym,
    procsym,
    writesym,
    readsym,
    elsesym
} token_type;

typedef struct
{
    int kind;      // const = 1, var = 2, proc = 3.
    char name[11]; // name up to 11 chars
    int val;       // number (ASCII value)
    int level;     // L  level
    int adr;       // M  address
} namerecord_t;

int check_for_reserved(char *word)
{
    if (strcmp(word, "call" || "begin" || "end" || "if" || "then" || "else" || "while" || "do" || "read" || "write") == 0)
        return 1;
    return 0;
}
int check_for_special(char word)
{
    if (word == "+" || "-" || "*" || "/" || "(" || ")" || "=" || "," || "." || "<" || ">" || ";" || ":")
        return 1;
    return 0;
}

/*main Program*/
int main(int argc, char **argv)
{
    FILE *inf;
    FILE *outF;

    // Reserved Words: call, begin, end, if, then, else, while, do, read, write.
    char *word[] = {"null", "begin", "call", "const", "do", "else", "end", "if",
                    "odd", "procedure", "read", "then", "var", "while", "write"};

    int wsym[] = {numbersym, beginsym, callsym, constsym, dosym, elsesym, endsym, ifsym,
                  oddsym, procsym, readsym, thensym, varsym, whilesym, writesym};

    // Special Symbols: ‘+’, ‘-‘, ‘*’, ‘/’, ‘(‘, ‘)’, ‘=’, ’,’ , ‘.’, ‘ <’, ‘>’,  ‘;’ , ’:’ .
    int ssym[256];
    ssym['+'] = "plus";
    ssym['-'] = "minus";
    ssym['*'] = "mult";
    ssym['/'] = "slash";
    ssym['('] = "lparen";
    ssym[')'] = "rparen";
    ssym['='] = "eql";
    ssym[','] = "comma";
    ssym['.'] = "period";
    ssym['#'] = "neq";
    ssym['<'] = "lss";
    ssym['>'] = "gtr";
    ssym['$'] = "leq";
    ssym['%'] = "geq";
    ssym[';'] = "semicolon";


    // opening File
    inf = fopen(argv[1], "r");
    if (inf == NULL)
    {
        printf("Error opening file!"); // checking file if it exists
        exit(1);
    }
    else
    {
        outF = fopen("output.txt", "r");
        char buffer;
        while (scanf("%[^\n]%*c", buffer))
        {
        }
    }
}