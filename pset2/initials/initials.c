#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{   
    // get name from user
    string name = get_string();
    // if the first character of a string is alpha then print it
    if (name[0] != ' ')
    {
        printf("%c", toupper(name[0]));
    }
    
    for (int i = 0, n = strlen(name); i < n; i++)
    {
        // print every first character in a word
        if (isalpha(name[i]) && name[i-1] == ' ' && i != 0)
        {
            printf("%c", toupper(name[i]));
        }
    }
    // new line
    printf("\n");
}