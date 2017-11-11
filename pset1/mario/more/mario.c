#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    // get height from user
    do
    {
        printf("Height: ");
        height = get_int();
    }
    while (!(height <= 23 && height >= 0));
    // print the pyramid
    for (int i = 1; i <= height; i++)
    {
        // print the first half of the pyramid
        for (int k = 0; k < height - i; k++)
        {
            printf(" ");
        }
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }
        // print the space in the middle of the pyramid
        for (int k = 0; k < 2; k++)
        {
            printf(" ");
        }
        // print the second half of the pyramid
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }
        // set the cursor to next line in the pyramid
        printf("\n");
    }
}