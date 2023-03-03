/*
    Feifan Cao
    Dr. Montagne
    COP3402 Spring
    HW1 - Implement and submit the P-machine(PM/0)
*/

#include <stdio.h>
#include <stdlib.h>

#define STACKSIZE 500
#define RECORDS 100

typedef struct instruction
{
    int op; // op code
    int l;  // lexicographical level
    int m;  // it's complicated
} ins;

typedef struct register_arc
{
    int bp; // point to the current record;
    int sp; // point to current top of the stack
    int pc; // points to next instruction to execute;
    ins ir; // stores the instruction
} res;

/**********************************************/
/*		Find base L levels down		 */
/*							 */
/**********************************************/

int base(int BP, int L, int myStack[])
{
    int arb = BP; // arb = activation record base

    while (L > 0) // find base L levels down
    {
        arb = myStack[arb];
        L--;
    }
    return arb;
}

int main(int argc, char **argv)
{
    FILE *inf;
    res myRes;             // Register instant
    ins myText[STACKSIZE]; // instruction to execute
    int myStack[STACKSIZE];

    inf = fopen(argv[1], "r");

    if (inf == NULL)
    {
        printf("Error opening file!"); // checking file if it exists
        exit(1);
    }
    else
    {
        myRes.pc = 0;
        myRes.bp = 499;
        myRes.sp = 500;

        int k = 0;
        int input;       // input for op = 9
        int op1, l1, m1; // temp variables to read input from the input file
        int halt = 0;    // flag to check if the program is running
        int lines = 0;
        int ar_exist = 0;

        for (int i = 0; i < STACKSIZE; i++)
            myStack[i] = 0; // making every element in the stack to be 0

        printf("        PC   BP   SP   stack\n");
        printf("Initial values:  %d  %d  %d\n\n", myRes.pc, myRes.bp, myRes.sp);

        while (fscanf(inf, "%d %d %d", &op1, &l1, &m1) == 3)
        {
            myText[lines].op = op1;
            myText[lines].l = l1;
            myText[lines].m = m1;
            lines++;
        }
        fclose(inf);

        while (k < lines && halt == 0)
        {
            myRes.ir.op = myText[k].op; // fetch from PAS text section
            myRes.ir.l = myText[k].l;
            myRes.ir.m = myText[k].m;
            myRes.pc += 3;

            if (myRes.ir.op == 1)
            {
                myStack[--myRes.sp] = myRes.ir.m;
                printf("  LIT  ");
            }
            else if (myRes.ir.op == 2)
            {
                if (myRes.ir.m == 1)
                {
                    myStack[myRes.sp + 1] = myStack[myRes.sp + 1] + myStack[myRes.sp];
                    myRes.sp++;
                    printf("  ADD  ");
                }
                else if (myRes.ir.m == 2)
                {
                    myStack[myRes.sp + 1] = myStack[myRes.sp + 1] - myStack[myRes.sp];
                    myRes.sp++;
                    printf("  SUB  ");
                }

                else if (myRes.ir.m == 3)
                {
                    myStack[myRes.sp + 1] = myStack[myRes.sp + 1] * myStack[myRes.sp];
                    myRes.sp++;
                    printf("  MUL  ");
                }
                else if (myRes.ir.m == 4)
                {
                    myStack[myRes.sp + 1] = myStack[myRes.sp + 1] / myStack[myRes.sp];
                    myRes.sp++;
                    printf("  DIV  ");
                }
                else if (myRes.ir.m == 5)
                {
                    myStack[myRes.sp + 1] = myStack[myRes.sp + 1] == myStack[myRes.sp];
                    myRes.sp++;
                    printf("  EQL  ");
                }
                else if (myRes.ir.m == 6)
                {
                    myStack[myRes.sp + 1] = myStack[myRes.sp + 1] != myStack[myRes.sp];
                    myRes.sp++;
                    printf("  NEQ  ");
                }
                else if (myRes.ir.m == 7)
                {
                    myStack[myRes.sp + 1] = myStack[myRes.sp + 1] < myStack[myRes.sp];
                    myRes.sp++;
                    printf("  LSS  ");
                }
                else if (myRes.ir.m == 8)
                {
                    myStack[myRes.sp + 1] = myStack[myRes.sp + 1] <= myStack[myRes.sp];
                    myRes.sp++;
                    printf("  LEQ  ");
                }
                else if (myRes.ir.m == 9)
                {
                    myStack[myRes.sp + 1] = myStack[myRes.sp + 1] > myStack[myRes.sp];
                    myRes.sp++;
                    printf("  GTR  ");
                }
                else if (myRes.ir.m == 10)
                {
                    myStack[myRes.sp + 1] = myStack[myRes.sp + 1] >= myStack[myRes.sp];
                    myRes.sp++;
                    printf("  GEQ  ");
                }
                else if(myRes.ir.m = 11)
                {
                    myStack[myRes.sp + 1] = myStack[myRes.sp + 1] % 2;
                    myRes.sp++;
                    printf(" ODD ");
                }
                else
                {
                    myRes.sp = myRes.bp + 1;
                    myRes.bp = myStack[myRes.sp - 2];
                    myRes.pc = myStack[myRes.sp - 3];
                    k = myStack[myRes.sp - 3] / 3 - 1;

                    ar_exist = 0;
                    printf("  RTN  ");
                }
            }
            else if (myRes.ir.op == 3)
            {
                myStack[--myRes.sp] = myStack[base(myRes.bp, myRes.ir.l, myStack) - myRes.ir.m];
                printf("  LOD  ");
            }
            else if (myRes.ir.op == 4)
            {
                myStack[base(myRes.bp, myRes.ir.l, myStack) - myRes.ir.m] = myStack[myRes.sp++];
                printf("  STO  ");
            }
            else if (myRes.ir.op == 5)
            {

                myStack[myRes.sp - 1] = base(myRes.bp, myRes.ir.l, myStack);
                myStack[myRes.sp - 2] = myRes.bp;
                myStack[myRes.sp - 3] = myRes.pc;
                myRes.bp = myRes.sp - 1;

                k = myRes.ir.m / 3 - 1;
                myRes.pc = myRes.ir.m;
                ar_exist = 1;

                printf("  CAL  ");
            }
            else if (myRes.ir.op == 6)
            {
                myRes.sp -= myRes.ir.m;
                printf("  INC  ");
            }
            else if (myRes.ir.op == 7)
            {
                k = myRes.ir.m / 3 - 1;
                myRes.pc = myRes.ir.m;

                printf("  JMP  ");
            }
            else if (myRes.ir.op == 8)
            {
                if (myStack[myRes.sp] == 0)
                {
                    k = myRes.ir.m / 3 - 1;
                    myRes.pc = myRes.ir.m;
                    myRes.sp++;
                    printf("  JPC  ");
                }
            }
            else if (myRes.ir.op == 9)
            {
                if (myRes.ir.m == 1)
                {
                    printf("Output result is: %d \n", myStack[myRes.sp]);
                    myRes.sp++;
                    printf("  Write  ");
                }
                else if (myRes.ir.m == 2)
                {
                    printf("Please Enter an Integer: ");
                    scanf("%d", &input);
                    myStack[--myRes.sp] = input;
                    printf("  Read  ");
                }
                else
                {
                    printf("  End of Program  ");
                    halt = 1;
                }
            }
            else
            {
            }

            printf(" %d    %d   %d    %d    %d    ", myRes.ir.l, myRes.ir.m, myRes.pc, myRes.bp, myRes.sp);
            for (int i = STACKSIZE - 1; i >= myRes.sp; i--)
            {
                if (i == myRes.bp && ar_exist == 1)
                    printf(" | ");
                printf("%d ", myStack[i]);
            }
            printf("\n");
            k++;
        }
    }

    return 0;
}