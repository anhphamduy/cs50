#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

#define FIRST_ORDER_UPPERCASE 65
#define FIRST_ORDER_LOWERCASE 97

bool not_valid(int length, string s);

int main(int argc, char *argv[])
{
    // check to see if enough arguments
    if (not_valid(argc, argv[1]))
    {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    
    // get plain text from user
    printf("plaintext: ");
    string plain_text = get_string();
    
    printf("ciphertext: ");
    // counter for the key
    int counter = 0;
    // loop through the plain text and add counter by 1 each loop
    for (int i = 0, n = strlen(plain_text); i < n; i++)
    {
        if (islower(plain_text[i]))
        {
             // get the key of a character
            int key;
            if (islower(argv[1][counter]))
                key = ((int) argv[1][counter]) - FIRST_ORDER_LOWERCASE;
            else
                key = ((int) argv[1][counter]) - FIRST_ORDER_UPPERCASE;
            // get the number of final character
            int final_letter = ((int) plain_text[i] - FIRST_ORDER_LOWERCASE + key) % 26 + FIRST_ORDER_LOWERCASE;
            printf("%c", (char) final_letter);
            // increment counter to move to next character in key
            counter += 1;
        }
        else if (isupper(plain_text[i]))
        {
            // get the key of a character
            int key;
            if (islower(argv[1][counter]))
                key = ((int) argv[1][counter]) - FIRST_ORDER_LOWERCASE;
            else
                key = ((int) argv[1][counter]) - FIRST_ORDER_UPPERCASE;
            // get the number of final character
            int final_letter = ((int) plain_text[i] - FIRST_ORDER_UPPERCASE + key) % 26 + FIRST_ORDER_UPPERCASE;
            printf("%c", (char) final_letter);
            // increment counter to move to next character in key
            counter += 1;
        }
        else {
            printf("%c", plain_text[i]);
        }
        // reset counter
        if (counter == strlen(argv[1]))
        {
            counter = 0;
        }
    }
    // new line
    printf("\n");
}

// return true to indicate that the arguments are not valid
bool not_valid(int length, string s)
{
    // check length
    if (length != 2)
        return true;
    
    // check all character in the second in argument vector
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isalpha(s[i]))
            return true;
    }
    // default return
    return false;
}