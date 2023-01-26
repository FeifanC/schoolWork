/*
    Feifan Cao, Juan M Garcia
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

typedef struct process_address_space
{
    ins Text[STACKSIZE];  // instruction to execute
    int stack[STACKSIZE]; // data-stack used by PM/0 CPU
} pas;

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

int base(int BP, int L, pas mypas)
{
    int arb = BP; // arb = activation record base

    while (L > 0) // find base L levels down
    {
        arb = mypas.stack[arb];
        L--;
    }
    return arb;
}

int main(int argc, char **argv)
{
    FILE *inf;
    res myRes; // Register instant
    pas myPAS; // process_address_space instant

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
            myPAS.stack[i] = 0; // making every element in the stack to be 0

        printf("        PC   BP   SP   stack\n");
        printf("Initial values:  %d  %d  %d\n\n", myRes.pc, myRes.bp, myRes.sp);

        while (fscanf(inf, "%d %d %d", &op1, &l1, &m1) == 3)
        {
            myPAS.Text[lines].op = op1;
            myPAS.Text[lines].l = l1;
            myPAS.Text[lines].m = m1;
            lines++;
        }
        fclose(inf);

        while (k < lines && halt == 0)
        {
            myRes.ir.op = myPAS.Text[k].op; // fetch from PAS text section
            myRes.ir.l = myPAS.Text[k].l;
            myRes.ir.m = myPAS.Text[k].m;
            myRes.pc += 3;

            if (myRes.ir.op == 1)
            {
                myPAS.stack[--myRes.sp] = myRes.ir.m;
                printf("  LIT  ");
            }
            else if (myRes.ir.op == 2)
            {
                if (myRes.ir.m == 1)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] + myPAS.stack[myRes.sp];
                    myRes.sp++;
                    printf("  ADD  ");
                }
                else if (myRes.ir.m == 2)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] - myPAS.stack[myRes.sp];
                    myRes.sp++;
                    printf("  SUB  ");
                }

                else if (myRes.ir.m == 3)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] * myPAS.stack[myRes.sp];
                    myRes.sp++;
                    printf("  MUL  ");
                }
                else if (myRes.ir.m == 4)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] / myPAS.stack[myRes.sp];
                    myRes.sp++;
                    printf("  DIV  ");
                }
                else if (myRes.ir.m == 5)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] == myPAS.stack[myRes.sp];
                    myRes.sp++;
                    printf("  EQL  ");
                }
                else if (myRes.ir.m == 6)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] != myPAS.stack[myRes.sp];
                    myRes.sp++;
                    printf("  NEQ  ");
                }
                else if (myRes.ir.m == 7)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] < myPAS.stack[myRes.sp];
                    myRes.sp++;
                    printf("  LSS  ");
                }
                else if (myRes.ir.m == 8)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] <= myPAS.stack[myRes.sp];
                    myRes.sp++;
                    printf("  LEQ  ");
                }
                else if (myRes.ir.m == 9)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] > myPAS.stack[myRes.sp];
                    myRes.sp++;
                    printf("  GTR  ");
                }
                else if (myRes.ir.m == 10)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] >= myPAS.stack[myRes.sp];
                    myRes.sp++;
                    printf("  GEQ  ");
                }
                else
                {
                    myRes.sp = myRes.bp + 1;
                    myRes.bp = myPAS.stack[myRes.sp - 2];
                    myRes.pc = myPAS.stack[myRes.sp - 3];
                    k = myPAS.stack[myRes.sp - 3] / 3 - 1;

                    ar_exist = 0;
                    printf("  RTN  ");
                }
            }
            else if (myRes.ir.op == 3)
            {
                myPAS.stack[--myRes.sp] = myPAS.stack[base(myRes.bp, myRes.ir.l, myPAS) - myRes.ir.m];
                printf("  LOD  ");
            }
            else if (myRes.ir.op == 4)
            {
                myPAS.stack[base(myRes.bp, myRes.ir.l, myPAS) - myRes.ir.m] = myPAS.stack[myRes.sp++];
                printf("  STO  ");
            }
            else if (myRes.ir.op == 5)
            {

                myPAS.stack[myRes.sp - 1] = base(myRes.bp, myRes.ir.l, myPAS);
                myPAS.stack[myRes.sp - 2] = myRes.bp;
                myPAS.stack[myRes.sp - 3] = myRes.pc;
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
                if (myPAS.stack[myRes.sp] == 0)
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
                    printf("Output result is: %d \n", myPAS.stack[myRes.sp]);
                    myRes.sp++;
                    printf("  Write  ");
                }
                else if (myRes.ir.m == 2)
                {
                    printf("Please Enter an Integer: ");
                    scanf("%d", &input);
                    myPAS.stack[--myRes.sp] = input;
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
                printf("%d ", myPAS.stack[i]);
            }
            printf("\n");
            k++;
        }
    }

    return 0;
}