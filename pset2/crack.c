#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>

int main(int argc, string argv[])
{   
    if (argc != 2)
    {
        printf("how can i steal your password if your hash is wrong?\n");
        return 1;
    }
    string hash;
    char list[54] = "\0abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char salt[3];    
    char word[6] = "";
    
    word[5] = '\0';
    strncpy(salt, argv[1], 2);
    salt[2] = '\0';
    // Password generation
    // 5th letter
    for (int i = 0;i < 53;i++)
    {
        // 4th letter
        for (int j = 0;j < 53;j++)
        {
            // 3rd letter
            for (int k = 0;k < 53;k++)
            {
                //2nd letter
                for (int l = 0;l < 53;l++)
                {
                    // 1st letter
                    for (int m = 1;m < 53;m++)
                    {
                        word[0] = list[m];
                        word[1] = list[l];
                        word[2] = list[k];
                        word[3] = list[j];
                        word[4] = list[i];
                        hash = crypt(word, salt);
                     //   printf("%s__", word);
                        if (strcmp(hash, argv[1]) == 0)
                        {
                            printf("%s\n", word);
                            return 1;
                        }
                    }
                }
            }
        }
    }    
}
