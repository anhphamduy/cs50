#include <stdio.h>

#define _XOPEN_SOURCE
#include <unistd.h>
#include <crypt.h>
#include <cs50.h>
#include <string.h>

int current_width = 1;
int current_cases = 2;

string make_string(int length, int array_of_number[]);
string check_string(int array_of_cases[current_cases][current_width], string hash, string salt);

int main(int argc, string argv[])
{
    // check for the argument count
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    
    // store hash in hash variable and check if it's valid
    string hash = argv[1];
    if (strlen(hash) != 13)
    {
        printf("Invalid hash\n");
        return 1;
    }
    
    // get the salt
    string salt = malloc(sizeof(char) * 3);
    salt[0] = hash[0];
    salt[1] = hash[1];
    salt[2] = '\0';

    // loop through 4 cases where we have one, two, three, four characte
    for (int length = 1; length < 5; length++)
    {
        // one chracter
        if (length == 1)
        {
            for (int i = 65; i <= 90; i++)
            {
                int cases[2][1] = {{i}, {i + 32}};
                if (strlen(check_string(cases, hash, salt)) != 0)
                    return 0;
            }
        }
        // two character
        else if (length == 2)
        {
            for (int i = 65; i <= 90; i++)
            {
                for (int k = 65; k <= 90; k++)
                {
                    int cases[4][2] = {{i, k}, {i + 32, k}, {i, k + 32}, {i + 32, k + 32}};
                    
                    current_width = 2;
                    current_cases = 4;
                    
                    if (strlen(check_string(cases, hash, salt)) != 0)
                        return 0;
                }
            }
        }
        // three character
        else if (length == 3)
        {
            for (int i = 65; i <= 90; i++)
            {
                for (int k = 65; k <= 90; k++)
                {
                    for (int m = 65; m <= 90; m++)
                    {
                        int cases[8][3] = {{i, k, m}, {i + 32, k, m}, {i, k + 32, m}, {i, k, m + 32}, {i + 32, k + 32, m}, 
                            {i, k + 32, m + 32}, {i + 32, k, m + 32}, {i + 32, k + 32, m + 32}
                        };
                        
                        current_width = 3;
                        current_cases = 8;
                        
                        if (strlen(check_string(cases, hash, salt)) != 0)
                            return 0;
                    }
                }
            }
        }
        // four character
        else if (length == 4)
        {
            for (int i = 65; i <= 90; i++)
            {
                for (int k = 65; k <= 90; k++)
                {
                    for (int m = 65; m <= 90; m++)
                    {
                        for (int n = 65; n <= 90; n++)
                        {
                            int cases[16][4] = {{i, k, m, n}, {i + 32, k, m, n}, {i, k + 32, m, n}, {i, k, m + 32, n}, {i, k, m, n + 32},
                                {i + 32, k + 32, m, n}, {i, k + 32, m + 32, n}, {i, k, m + 32, n + 32}, {i + 32, k, m, n + 32}, {i + 32, k, m + 32, n},
                                {i, k + 32, m, n + 32}, {i + 32, k + 32, m + 32, n}, {i, k + 32, m + 32, n + 32}, {i + 32, k + 32, m, n + 32},
                                {i + 32, k, m + 32, n + 32}, {i + 32, k + 32, m + 32, n + 32}};
                            
                            current_width = 4;
                            current_cases = 16;
                            
                            if (strlen(check_string(cases, hash, salt)) != 0)
                                return 0;
                        }
                    }
                    
                }
            }
        }
    }
    
}
// make new string depending on the ASCII number of a digit
string make_string(int length, int array_of_number[])
{
    string s = malloc(sizeof(char) * (length + 1));
    for (int counter = 0; counter < length; counter++)
    {
        s[counter] = (char) array_of_number[counter];
    }
    // string terminator
    s[length] = '\0';
    return s;
}
// check to see if it matches the hash
string check_string(int array_of_cases[current_cases][current_width], const string hash, const string salt)
{
    for (int i = 0; i < current_cases; i++)
    {
        if (strcmp(crypt(make_string(current_width, array_of_cases[i]), salt), hash) == 0)
        {
            printf("%s\n", make_string(current_width, array_of_cases[i]));
            return make_string(current_width, array_of_cases[i]);
        }
    }
    return "";
}