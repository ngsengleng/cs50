// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// function prototypes
int makeHash(const char c);
int countWords(node *n);
void erase(node *n);

// Represents a trie
node *root;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;

    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];
    int what = 0;

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        node *n = root;

        for (int i = 0, length = strlen(word); i  < length; i++)
        {
            char c = word[i];
            int key = makeHash(c);
            if (n->children[key] == NULL)
            {
                // create new node
                n->children[key] = malloc(sizeof(node));
                n->children[key]->is_word = false;
                for (int j = 0; j < N; j++)
                {
                    n->children[key]->children[j] = NULL;
                }
            }
            n = n->children[key];
        }
        n->is_word = true;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    int num = countWords(root);
    return num;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *n = root;
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        int key = makeHash(word[i]);
        if (n->children[key] == NULL)
            return false;
        n = n->children[key];
    }
    return n->is_word;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    erase(root);
    return true;
}




/////////////////////
// MY SUB FUNCTIONS//
/////////////////////


int makeHash(const char c)
{
    if (islower(c))
    {
        return c - 'a';
    }
    else if (isupper(c))
    {
        return c - 'A';
    }
    else if (c == '\'')
    {
        return N - 1;
    }
    else
    {
        return -1;
    }
}

int countWords(node *n)
{
    int count = 0;
    if (n)
    {
        if (n->is_word == true)
        {
            count++;
        }
        for (int i = 0; i < N; i++)
        {
            if (n->children[i] != NULL)
            {
                count += countWords(n->children[i]);
            }
        }
    }
    return count;
}

void erase(node *n)
{
    for (int i = 0; i < N; i++)
    {
        if (n->children[i])
        {
            erase(n->children[i]);
        }

    }
    free(n);
    return;
}
