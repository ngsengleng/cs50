// Shift cipher
#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

char change(char wordstr,int b);

int main(int argc, string argv[])
{
    // For no inputs
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    int a = atoi(argv[1]) % 26;
    // Userinput for non-numbers and wrong key inputs
    for (int i = 0;i < strlen(argv[1]);i++)
    {
        if (isdigit(argv[1][i]) == 0)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    // Body
    string wordstr = get_string("plaintext: ");
    printf("ciphertext: ");
    for (int i = 0;i < strlen(wordstr);i++)
    {
        if (islower(wordstr[i]))
        {
            wordstr[i] = change(wordstr[i], a);
        }
        else if (isupper(wordstr[i]))
        {
            wordstr[i] = change(wordstr[i], a);
        }
        printf("%c", wordstr[i]);
    }
    printf("\n");
}

char change(char wordstr, int b)
{
    // Lowercase
    if (wordstr + b > 'Z' && wordstr + b > 'z')
    {
        wordstr += b - 26;
    }
    // Uppercase
    else if (wordstr + b > 'Z' && wordstr + b < 'a')
    {
        wordstr += b - 26;
    }
    else
    {
        wordstr += b; 
    }
    return wordstr;
}
