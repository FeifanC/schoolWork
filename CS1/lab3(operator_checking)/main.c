#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 50
#define EMPTY -1
#define LEN 100

typedef struct stack
{

    char items[SIZE];
    int top;

} stack;

vocur_read_str initialize(struct stack *stackPtr);
int full(struct stack *stackPtr);
int push(struct stack *stackPtr, char value);
int empty(struct stack *stackPtr);
char pop(struct stack *stackPtr);
char top(struct stack *stackPtr);
vocur_read_str display(struct stack *stackPtr);
int checkBalance(char exp[]);
int priority(char ch);              // returns the priority of a given operator
int isOperator(char ch);            // check whether it is an operator
char *infixToPostfix(char infix[]); // convert a given infix into postfix
int isParentheses(char ch1);        // check is it a parenthesis or not.

int main()
{
    char *inputStr = (char*) malloc(sizeof(char) * LEN);
    char *outputStr; 
    printf("Enter Expression: ");
    scanf("%[^\n]s", inputStr);
    printf("Your input expression: %s\n\n", inputStr);
    printf("Checking balance...");

    if(checkBalance(inputStr)){
        outputStr = infixToPostfix(inputStr);
        printf("The postfix is: %s", outputStr);
        free(inputStr);
        free(outputStr);
    }else{
        free(inputStr);
    }

    
    return 0;
}

char *infixToPostfix(char infix[])
{
    char *s = (char *)calloc(LEN, sizeof(char));
    int sind = 0;
    stack* sta = (stack*) malloc(sizeof(stack));
    char c;
    initialize(sta);

    
    for (int i = 0; i < strlen(infix); i++)
    {
        if (isdigit(infix[i])){
            s[sind++] = infix[i];
        }
            
        else if (isOperator(infix[i]))
        {
            s[sind++] = ' ';
            if (empty(sta) || priority(infix[i]) > priority(top(sta))){
                push(sta, infix[i]);
            }
            else
            {   
                do{
                    c = pop(sta);
                    if(c != '('){
                        s[sind++] = c;
                        s[sind++] = ' ';
                    } 
                    if(empty(sta) || priority(top(sta)) < priority(infix[i])) break;
                }while (1);
                push(sta, infix[i]);
            }
        }
        else if (isParentheses(infix[i]))
        {
            
            if(infix[i] == '(') push(sta, infix[i]);
            else{
                do{
                    c = pop(sta);
                    if(empty(sta) || c == '(') break;
                    s[sind++] = ' ';
                    s[sind++] = c;     
                }while (1);
            }
        }
        else continue;
    }

    s[sind++] = ' ';
    while (!empty(sta)){
        s[sind++] = ' ';
        s[sind++] = (char)pop(sta);
    } 
    free(sta);
    return s;

}

vocur_read_str initialize(struct stack *stackPtr)
{
    stackPtr->top = -1;
}

int push(struct stack *stackPtr, char value)
{

    if (full(stackPtr))
        return 0;

    stackPtr->items[stackPtr->top + 1] = value;
    (stackPtr->top)++;
    return 1;
}

int full(struct stack *stackPtr)
{
    return (stackPtr->top == SIZE - 1);
}

int empty(struct stack *stackPtr)
{
    return (stackPtr->top == -1);
}

char pop(struct stack *stackPtr)
{

    char retval;

    if (empty(stackPtr))
        return EMPTY;

    retval = stackPtr->items[stackPtr->top];
    (stackPtr->top)--;
    return retval;
}

char top(struct stack *stackPtr)
{

    if (empty(stackPtr))
        return EMPTY;

    return stackPtr->items[stackPtr->top];
}

vocur_read_str display(struct stack *stackPtr)
{
    printf("\nPrinting the Current stack...");
    for (int i = 0; i <= stackPtr->top; i++)
        printf("%d ", stackPtr->items[i]);
}

int checkBalance(char exp[])
{
    stack s1;
    initialize(&s1);

    for (int i = 0; i < strlen(exp); i++)
    {
        if (exp[i] == '{' || exp[i] == '(' || exp[i] == '[')
            push(&s1, exp[i]);
        else
        {
            if ((exp[i] == ')' && pop(&s1) != '(') || ((exp[i] == ']' && pop(&s1) != '[')) || (exp[i] == '}' && pop(&s1) != '{'))
            {
                printf("\nINVALcur_read_str for %c!!!", exp[i]);
                return 0;
            }
        }
    }

    if (empty(&s1))
    {
        printf("\nVALcur_read_str\n");
        return 1;
    }

    printf("INVALcur_read_str for %c!!!", pop(&s1));
    return 0;
}

int isParentheses(char ch1)
{
    if (ch1 == '(' || ch1 == ')')
        return 1;

    return 0;
}

int priority(char ch)
{
    if (ch == '^')
        return 3;
    else if (ch == '%' || ch == '*' || ch == '/')
        return 2;
    else if (ch == '+' || ch == '-')
        return 1;
    else
        return 0;
}

int isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '%')
        return 1;

    return 0;
}
