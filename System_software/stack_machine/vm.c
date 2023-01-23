/*
    Feifan Cao, Juan M Garcia
    Dr. Montagne
    COP3402 Spring
    HW1 - Implement and submit the P-machine(PM/0)
*/

#include <stdio.h>
#include <stdlib.h>

#define STACKSIZE 500

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
    res myRes;
    pas myPAS;

    inf = fopen(argv[1], "r");

    if (inf == NULL)
    {
        printf("Error!");
        exit(1);
    }
    else
    {
        myRes.pc = 0;
        myRes.bp = 499;
        myRes.sp = 500;
        int input;

        for (int i = 0; i < STACKSIZE; i++)
            myPAS.stack[i] = 0;

        printf("                PC   BP   SP   stack\n");
        printf("Initial values:  %d  %d  %d\n\n", myRes.pc, myRes.bp, myRes.sp);

        while (fscanf(inf, "%d %d %d", &myRes.ir.op, &myRes.ir.l, &myRes.ir.m) == 3)
        {
            if (myRes.ir.op == 1)
            {
                myPAS.stack[--myRes.sp] = myRes.ir.m;
                printf("  LIT  ");
            }
            else if (myRes.ir.op == 2)
            {
                if (myRes.ir.m == 1)
                {
                    myPAS.stack[++myRes.sp] = myPAS.stack[++myRes.sp] + myPAS.stack[myRes.sp - 1];
                }
                else if (myRes.ir.m == 2)
                {
                    myPAS.stack[++myRes.sp] = myPAS.stack[++myRes.sp] - myPAS.stack[myRes.sp - 1];
                }
                else if (myRes.ir.m == 3)
                {
                    myPAS.stack[++myRes.sp] = myPAS.stack[++myRes.sp] * myPAS.stack[myRes.sp - 1];
                }
                else if (myRes.ir.m == 4)
                {
                    myPAS.stack[++myRes.sp] = myPAS.stack[++myRes.sp] / myPAS.stack[myRes.sp - 1];
                }
                else if (myRes.ir.m == 5)
                {
                    myPAS.stack[++myRes.sp] = myPAS.stack[++myRes.sp] == myPAS.stack[myRes.sp - 1];
                }
                else if (myRes.ir.m == 6)
                {
                    myPAS.stack[++myRes.sp] = myPAS.stack[++myRes.sp] != myPAS.stack[myRes.sp - 1];
                }
                else if (myRes.ir.m == 7)
                {
                    myPAS.stack[++myRes.sp] = myPAS.stack[++myRes.sp] < myPAS.stack[myRes.sp - 1];
                }
                else if (myRes.ir.m == 8)
                {
                    myPAS.stack[++myRes.sp] = myPAS.stack[++myRes.sp] <= myPAS.stack[myRes.sp - 1];
                }
                else if (myRes.ir.m == 9)
                {
                    myPAS.stack[++myRes.sp] = myPAS.stack[++myRes.sp] > myPAS.stack[myRes.sp - 1];
                }
                else if (myRes.ir.m == 10)
                {
                    myPAS.stack[++myRes.sp] = myPAS.stack[++myRes.sp] >= myPAS.stack[myRes.sp - 1];
                }
                else
                {
                }
                printf("  RTN  ");
            }
            else if (myRes.ir.op == 3)
            {
                myPAS.stack[--myRes.sp] = myPAS.stack[base(myRes.bp, myRes.ir.l, myPAS) - myRes.ir.m];
                printf("  LOD  ");
            }
            else if (myRes.ir.op == 4)
            {
                myPAS.stack[base(myRes.bp, myRes.ir.l, myPAS) - myRes.ir.m] = myPAS.stack[myRes.sp];
                myRes.sp++;
                printf("  STO  ");
            }
            else if (myRes.ir.op == 5)
            {
                myPAS.stack[myRes.bp - 1] = base(myRes.bp, myRes.ir.l, myPAS);
                myPAS.stack[myRes.bp - 2] = myRes.bp;
                myPAS.stack[myRes.bp - 3] = myRes.pc;
                myRes.bp = myRes.sp - 1;
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
                    printf("  SOU  ");
                }
                else if (myRes.ir.m == 2)
                {
                    printf("Please Enter an Integer: ");
                    scanf("%d", &input);
                    myPAS.stack[myRes.sp] = input;
                    myRes.sp++;
                    printf("  SIN  ");
                }
                else
                {
                    printf("  EOP  ");
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

            printf(" %d   %d   %d   %d    %d      ", myRes.ir.l, myRes.ir.m, myRes.pc, myRes.bp, myRes.sp);
            myRes.pc += 3;
            for (int i = myRes.sp; i <= myRes.bp; i++)
            {
                printf("%d ", myPAS.stack[i]);
            }
            printf("\n");
        }
        fclose(inf);
    }

    return 0;
}