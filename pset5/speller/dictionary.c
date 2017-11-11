/**
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"
#define NODE_STORAGE 1000000

// Prototype
int hash_letter(const char letter);
char hash_number(const int number);

// Tries node structure
typedef struct node
{
    bool is_word;
    struct node* children[27];
}
node;

int words_dic = 0;
node *parent = NULL;
node *storage = NULL;
bool loaded = false;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // node to keep track of current letter
    node *current = parent;

    // iterate through all the letters in word
    for (int i = 0; i < strlen(word); i++)
    {
        // when in last word
        if (i == strlen(word) - 1)
        {
            // check to see if the last node to store last letter is NULL
            if (current->children[hash_letter(tolower(word[i]))] == NULL)
                return false;
            // if not null then check to see if it is a complete word
            else if (current->children[hash_letter(tolower(word[i]))]->is_word)
                return true;
        }
        //  ensure that next letter exists
        else if (current->children[hash_letter(tolower(word[i]))] == NULL)
            return false;


        // update current
        current = current->children[hash_letter(tolower(word[i]))];
    }

    return false;;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // allocate memory for parent tree and storage branch
    parent = calloc(1, sizeof(node));
    storage = calloc(NODE_STORAGE, sizeof(node));
    // ensure that there is enogh memory for parent and storage trees
    if (parent == NULL)
        return false;

    // open dictionary
    FILE *dic = fopen(dictionary, "r");

    // index to keep track of the current poistion in a word
    int index = 0;

    // variable to keep track of current node
    node *current = parent;

    if (storage == NULL)
    {
        return false;
    }

    // current capacity for node
    int current_capacity = 0;

    // iterate through every character
    for (int c = fgetc(dic); c != EOF; c = fgetc(dic))
    {
        // allow only alphabeltical characters and apostrophes
        if (isalpha(c) && index == 0 && c != '\'')
        {
            // if next letter is NULL and the allocate memory for next letter
            if (current->children[hash_letter(c)] == NULL)
            {
                current->children[hash_letter(c)] = &storage[current_capacity];
                current_capacity++;
            }
            // update index and current
            current = current->children[hash_letter(c)];
            index++;
        }
        else if (isalpha(c) || (c == '\'' && index > 0))
        {
            // if next letter is NULL and the allocate memory for next letter
            if (current->children[hash_letter(c)] == NULL)
            {
                current->children[hash_letter(c)] = &storage[current_capacity];
                current_capacity++;
            }
            // update index and current
            current = current->children[hash_letter(c)];
            index++;
        }
        else if (index > 0)
        {
            // terminate current word and index
            current->is_word = true;
            index = 0;

            // update current and add one to total words in dictioanry
            current = parent;
            words_dic++;
        }
    }

    // close dictionary and return loaded
    loaded = true;
    fclose(dic);
    return loaded;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (loaded)
        return words_dic;
    return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    free(storage);
    free(parent);
    return true;
}

int hash_letter(const char letter)
{
    if (letter == '\'')
        return 26;
    return (int) tolower(letter) - 97;
}

char hash_number(const int number)
{
    if (number == 26)
    {
        return '\'';
    }
    return (char) (number + 97);
}