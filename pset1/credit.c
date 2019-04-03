#include <stdio.h>
#include <cs50.h>
#include <math.h>

int invalid(long num);
int chkVal(long num);
long legit(long num);
void reply(long real, int card);

// // visa is 1, amex is 2, mastercard is 3
int main(void)
{   
    long num;
    
    // Takes input
    do
    {
        num = get_double("Number: ");
    }
    while (num < 1);
    //int result = invalid(num);
    if (num / (long) pow(10,13) == 0)
    {
        printf("INVALID\n");
    }
    else
    {
        int card = chkVal(num);
        long real = legit(num);
        reply(real, card); 
    }
}

// Differentiate the card
int chkVal(long num)
{
    int value = 0;
    long input = num;
    
    do
    {
        // Check for visa
        if (input/(long) pow(10,12) == 4 || input/(long) pow(10,15) == 4)
        {
            value = 1;
        }
        // Check for Amex
        else if (input/(long) pow(10,13) == 34 || input/(long) pow(10,13) == 37)
        {
            value = 2;
        }
        // Check for Mastercard
        else if ((input/(long) pow(10,14) > 50 && input/(long) pow(10,14) < 56) || input/(long) pow(10,14) == 22)
        {
            value = 3;
        }
    }
    while (value > 4);
    return value;
}

// Authenticity of card
long legit(long num)
{
    int m  = 2;
    int n = 1;
    int digit = 0;
    long sum1 = 0, sum2 = 0, Tsum = 0;

    while (m < 17)
    {
        long j = num, k = num;
        // Authentication part1
        for (int i = 1;i < m;i++)
        {
            j = j / 10;
        }
        j = j % 10;
        digit = (j * 2) / 10 + (j * 2) % 10;
        sum1 = sum1 + digit;
        // Authentication part2
        for (int Count = 2;Count != m; Count++)
        {
            k = k / 10;
        }
        k = k % 10;
        sum2 = sum2 + k;
        m = m + 2;
    }
    Tsum = sum1 + sum2;
    return Tsum;
}

// Generates a reply
void reply(long real, int card)
{
    if (card == 1 && (real % 10) == 0)
    {
        printf("VISA\n");
    }
    else if (card == 2 && (real % 10) == 0)
    {
        printf("AMEX\n");
    }
    else if (card == 3 && (real % 10) == 0)
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

