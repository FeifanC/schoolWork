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
    ins Text;             // instruction to execute
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
        myRes.sp = 500; // initilization
        int input;
        int jumpLines = 0;
        int ARs[RECORDS];
        int arSize = 0;

        for (int i = 0; i < STACKSIZE; i++)
            myPAS.stack[i] = 0; // making every element in the stack to be 0

        for (int i = 0; i < RECORDS; i++)
            ARs[i] = 0; // array to store active record

        printf("        PC   BP   SP   stack\n");
        printf("Initial values:  %d  %d  %d\n\n", myRes.pc, myRes.bp, myRes.sp);

        while (fscanf(inf, "%d %d %d", &myRes.ir.op, &myRes.ir.l, &myRes.ir.m) == 3)
        {
            if (--jumpLines > 0)
            {
                continue; // instructions should skip if the opCode is jump
            }

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
                }
                else if (myRes.ir.m == 2)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] - myPAS.stack[myRes.sp];
                }
                else if (myRes.ir.m == 3)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] * myPAS.stack[myRes.sp];
                }
                else if (myRes.ir.m == 4)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] / myPAS.stack[myRes.sp];
                }
                else if (myRes.ir.m == 5)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] == myPAS.stack[myRes.sp];
                }
                else if (myRes.ir.m == 6)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] != myPAS.stack[myRes.sp];
                }
                else if (myRes.ir.m == 7)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] < myPAS.stack[myRes.sp];
                }
                else if (myRes.ir.m == 8)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] <= myPAS.stack[myRes.sp];
                }
                else if (myRes.ir.m == 9)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] > myPAS.stack[myRes.sp];
                }
                else if (myRes.ir.m == 10)
                {
                    myPAS.stack[myRes.sp + 1] = myPAS.stack[myRes.sp + 1] >= myPAS.stack[myRes.sp];
                }
                else
                {
                }
                myRes.sp = myRes.bp + 1;
                myRes.bp = myPAS.stack[myRes.sp - 2];
                myRes.pc = myPAS.stack[myRes.sp - 3];
                printf("  RTN  ");
            }
            else if (myRes.ir.op == 3)
            {
                if (arSize > 0)
                    ARs[arSize++] = myPAS.stack[base(myRes.bp, myRes.ir.l, myPAS) - myRes.ir.m];
                else
                    myPAS.stack[--myRes.sp] = myPAS.stack[base(myRes.bp, myRes.ir.l, myPAS) - myRes.ir.m];
                printf("  LOD  ");
            }
            else if (myRes.ir.op == 4)
            {
                if (arSize > 0)
                {
                    ARs[arSize++] = myPAS.stack[base(myRes.bp, myRes.ir.l, myPAS) - myRes.ir.m];
                    myRes.sp--;
                }
                else
                {
                    myPAS.stack[base(myRes.bp, myRes.ir.l, myPAS) - myRes.ir.m] = myPAS.stack[myRes.sp];
                    myRes.sp++;
                }
                printf("  STO  ");
            }
            else if (myRes.ir.op == 5)
            {
                ARs[arSize++] = base(myRes.bp, myRes.ir.l, myPAS);
                ARs[arSize++] = myRes.bp;
                ARs[arSize++] = myRes.pc;
                arSize += (myRes.ir.m - arSize);
                // myRes.bp = myRes.sp - 1;
                myRes.pc = myRes.ir.m;
                printf("  CAL  ");
            }
            else if (myRes.ir.op == 6)
            {
                myRes.sp -= myRes.ir.m;
                printf("  INC  ");
            }
            else if (myRes.ir.op == 7)
            {
                myPAS.stack[myRes.sp - 1] = 0;
                myRes.pc = myRes.ir.m;
                printf("  JMP  ");
                jumpLines = myRes.ir.m / 3;
            }
            else if (myRes.ir.op == 8)
            {
                printf("  JPC  ");
                if (myPAS.stack[myRes.sp] == 0)
                {
                    myRes.pc = myRes.ir.m;
                    myRes.sp++;
                }
            }
            else if (myRes.ir.op == 9)
            {
                if (myRes.ir.m == 1)
                {
                    printf("%c ", (char)(myPAS.stack[myRes.sp]));
                    myRes.sp++;
                    printf("  Write  ");
                }
                else if (myRes.ir.m == 2)
                {
                    printf("Please Enter an Integer: ");
                    scanf("%d", &input);
                    myPAS.stack[STACKSIZE - 1] = input;
                    myRes.sp--;
                    printf("  Read  ");
                }
                else
                {
                    printf("  End of Program  ");
                    printf(" %d    %d   %d    %d    %d      ", myRes.ir.l, myRes.ir.m, myRes.pc, myRes.bp, myRes.sp);
                    myRes.pc += 3;
                    for (int i = myRes.sp; i < myRes.bp; i++)
                    {
                        printf("%d ", myPAS.stack[i]);
                    }
                    printf("\n");
                    break;
                }
            }
            else
            {
            }
            printf(" %d   %d   %d   %d    %d      ", myRes.ir.l, myRes.ir.m, myRes.pc, myRes.bp, myRes.sp); // output the stack
            myRes.pc += 3;                                                                                  // program counter increase by 3 every line got read
            for (int i = myRes.sp; i <= myRes.bp; i++)
                printf("%d ", myPAS.stack[i]);
            if (arSize > 0) // printing activiation record
            {
                printf("|");
                for (int i = 0; i < arSize; i++)
                {
                    printf("%d ", ARs[i]);
                }
            }

            printf("\n");
        }
        fclose(inf);
    }

    return 0;
}