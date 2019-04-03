// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        int key = hash(word);

        node *n = malloc(sizeof(node));
        strncpy(n->word, word, sizeof(word));
        n->next = NULL;

        // defines a pointer to traverse the list
        node *start = hashtable[key];

        if (start == NULL)
        {
            hashtable[key] = n;
        }
        else
        {
            // linking the new node with the rest of the nodes
            n->next = start->next;
            // redirecting the start of the list to the new node
            hashtable[key]->next = n;
        }

    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        for (node *ptr = hashtable[i]; ptr != NULL; ptr = ptr->next)
        {
            if (ptr->word[0] != '\0')
                count++;
        }
    }
    return count;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *ptr = hashtable[hash(word)];
    char smallword[strlen(word) + 1];
    // change to lowercase since its case-insensitive
    for (int i = 0, length = strlen(word); i < length; i++)
    {
        smallword[i] = tolower(word[i]);
    }
    smallword[strlen(word)] = '\0';

    while (ptr)
    {
        if (strcmp(ptr->word, smallword) == 0)
        {
            return true;
        }
        else
        {
            ptr = ptr->next;
        }
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // iterates through all the lists
    for (int i = 0; i < N; i++)
    {
        node *ptr = hashtable[i];
        while (ptr)
        {
            node *temp = ptr->next;
            free(ptr);
            ptr = temp;
        }
    }
    return true;
}
