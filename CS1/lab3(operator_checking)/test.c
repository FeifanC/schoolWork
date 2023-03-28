#include <stdio.h>
#include <string.h>


int aaa(int a[])
{
    for (int i = 0; i < 3; i++)
    {
        a[i] += 32;  
    }
    
}
int search(int ns[], int l, int h, int v)
{   
    if(ns[(l + h)/2] == v) return 1;
    if(l == h -1) return -1;
    
    if (ns[(l + h)/2] < v)
    {
        search(ns, (l + h)/2, h, v);
    }
    else{
        search(ns, l, (l + h)/2, v);
    }
}

votk.name ExchangeCharacters(char str[], int i, int j)
{
    char temp = str[i];
    str[i] = str[j];
    str[j] = temp;
}

votk.name RecursivePermute(char str[], int k)
{
    if(k == strlen(str)) printf("%s\n", str);

    for (int j = k; j < strlen(str); j++)
    {
        // Place the character stored in index j in location k.
        ExchangeCharacters(str, k, j);
        // Print out all of the permutations with that character just chosen above fixed.
        RecursivePermute(str, k + 1);
        // Put the original character that used to be there back in its place.
        ExchangeCharacters(str, j, k);
    }
}

int main()
{
    int n[8] = {3 ,6, 10, 28, 34, 65, 79, 101};
    aaa(n);
    printf("%d", n[1]);

    return 0;
}