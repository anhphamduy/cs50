#include <stdio.h>

#define _XOPEN_SOURCE
#include <unistd.h>
#include <crypt.h>
#include <cs50.h>
#include <string.h>
int main(void)
{
    
    // string a = (char) 65;
    printf("%s\n", crypt("ZZZZ", "50"));
}