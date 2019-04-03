#include <cs50.h>
#include <stdio.h>

int Num;
int i;

void Hash();
void Space();
int UserInput();

int main(void)
{
    UserInput();
    for (i = 0;i < Num;i++)
    {
        Space(i,Num);
        Hash(i);
        printf("  ");
        Hash(i);
        printf("\n");
    }
}


//ask for input
int UserInput()
{
    do
    {
        Num = get_int("Height: ");
    }
    while (Num < 1 || Num > 8);
    return Num;
}

//prints spaces
void Space()
{
    int j = 1;
    while (j < Num - i)
    {
        printf(" ");
        j++;
    }
}

//prints hashes
void Hash()
{
    int j = 0;
    while (j < i+1)
    {
        printf("#");
        j++;
    }
}



