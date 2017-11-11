#include <stdio.h>
#include <cs50.h>

int get_length(long long card_number);
bool check_valid(long long number, int length);
int checktype(long long number, int length);

int main(void)
{
    printf("Number: ");
    long long card_number = get_long_long();
    // get length of card
    int length_of_card = get_length(card_number);
    
    // check the card valid
    if (check_valid(card_number, length_of_card))
    {
        // check the type and print its result
        int type = checktype(card_number, length_of_card);
        if (type == 4)
            printf("INVALID\n");
        else if (type == 3)
            printf("VISA\n");
        else if (type == 2)
            printf("MASTERCARD\n");
        else 
            printf("AMEX\n");
    }
    else
        printf("INVALID\n");
}

// function that returns the length of an integer series
int get_length(long long card_number)
{
    int length = 0;
    while (true)
    {
        card_number = card_number / 10;
        length += 1;
        if (card_number == 0)
            break;
    }
    return length;
}

bool check_valid(long long number, int length)
{
    // variable for containing the sum
    int sum = 0;
    // calculate the sum
    for (int i = 1; i <= length; i++)
    {
        // get the current number
        int num = number % 10;
        // if the current number is every second digit
        if (i % 2 == 0)
        {
            // double it
            num *= 2;
            // add to the sum
            while (true)
            {
                sum += (num % 10);
                num /= 10;
                if (num == 0)
                    break;
            }
        }
        // add the normal digit to sum
        else 
            sum += num;
        // get rid of last digit as all done
        number /= 10;
    }
    
    // return true if divisible by 10 otherwise false
    if (sum % 10 == 0)
        return true;
    return false;
}

// check type of the card
int checktype(long long number, int length)
{
    // get the first two digits 
    for (int i = 0; i < length; i++)
    {
        if (i == length - 2)
            break;
        number = number / 10;
    }
    
    // check the first two digits and the card's length
    if (number / 10 == 4 && (length == 13 || length == 16))
        return 3;
    else if ((number == 51 || number == 52 || number == 53 || number == 54 || number == 55) && length == 16)
        return 2;
    else if ((number == 34 || number == 37) && length == 15)
        return 1;
    return 4;
}