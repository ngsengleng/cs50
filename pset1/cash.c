#include <stdio.h>
#include <cs50.h>
#include <math.h>


float UserInput(void);
int Convert(float Dollar);
void Change(int Cents);


int main(void)
{
    float Dollar = UserInput();
    int Cents = Convert(Dollar);
    Change(Cents);
}


// Checks User Input
float UserInput(void)
{
    float Input;
    do
    {
        Input = get_float("Change owed: "); 
    }
    while (Input < 0 || Input == 0);
    return Input;
}


// Convert Dollars to Cents
int Convert(float Dollar)
{
    int Cent = round(Dollar * 100);
    return Cent;
}

// Counts Change
void Change(int Cents)
{
    int Quarter,Dime,Nickel,Penny,Sum;
    Quarter = Cents / 25;
    Dime = Cents % 25 / 10;
    Nickel = Cents % 25 % 10 / 5;
    Penny = Cents % 25 % 10 % 5;
    Sum = Quarter + Dime + Nickel + Penny;
    printf("%d\n", Sum);
}
