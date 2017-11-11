#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // get minutes from user
    printf("Minutes: ");
    int minute = get_int();
    
    // convert to bottle
    printf("Bottles: %i\n", minute * 12);
}