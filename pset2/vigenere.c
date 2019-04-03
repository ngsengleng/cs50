#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int position(char key);
char change(char wordstr,int b);

int main(int argc, string argv[])
{
    // Check if command line input is correct
    if (argc != 2)
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
    int len = strlen(argv[1]);
    int j = 0;
    string key = argv[1];
    string input;
    // Check for correct command line input
    for (int i = 0;i < len;i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
            printf("Usage: ./vigenere keyword\n");
            return 1;
        }
    }
    
    // Get plaintext
    input = get_string("plaintext: ");
    printf("ciphertext: ");
    // Encryption
    for (int i = 0;i< strlen(input);i++)
    {
        // Check if end of key is reached
        if (j == len)
        {
            j = 0;
        }
        // Check if input is a letter
        if (isalpha(input[i]))
        {
            input[i] = change(input[i], position(key[j]));
            j++;
        }
        printf("%c", input[i]);
    }
    printf("\n");
}

// Calculate key to cipher shift
int position(char key)
{
    int pos = 0;
    if (isupper(key))
    {
        key -= 'A';
        pos = (int) key;
    }
    else
    {
        key -= 'a';
        pos = (int) key;
    }
    return pos;
}

// Cipher shift
char change(char input, int b)
{
    // Lowercase
    if (islower(input) && input + b > 'z')
    {
        input += b - 26;
    }
    // Uppercase
    else if (input + b > 'Z' && isupper(input))
    {
        input += b - 26;
    }
    else
    {
        input += b; 
    }
    return input;
}


