/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 * Using binary search algorithm
 */
bool search(int value, int values[], int n)
{
    // do a linear seach
    if (n == 3 || n == 2)
    {
        for (int i = 0; i < n; i++)
            if (values[i] == value)
                return true;
        return false;
    }

    // get the midpoint
    int mid = -1;
    if (n % 2 == 0)
        mid = n / 2;
    else
        mid = (n - 1) / 2;

    // compare the midpoint to searched value
    if (values[mid] == value)
        return true;
    // check to see if the value it's greater or less than the midpoint
    if (value > values[mid])
    {
        // creating new array
        int new_array[n - mid];
        int counter = 0;
        for (int i = n - mid; i < n; i++)
        {
            new_array[counter] = values[i];
            counter += 1;
        }
        // recursive search
        if (search(value, new_array, n - mid))
            return true;
    }
    else
    {
        // creating new array
        int new_array[mid];
        for (int i = 0; i < mid; i++)
            new_array[i] = values[i];
        // recursive search
        if (search(value, new_array, mid))
            return true;
    }


    return false;
}

/**
 * Sorts array of n values using counting sort
 */
void sort(int values[], int n)
{
    // find the maximum value
    int maximum_value = values[0];
    for (int i = 0; i < n; i++)
    {
        if (values[i] > maximum_value)
            maximum_value = values[i];
    }

    // initialize the array for contaiining the counting part and set it to 0
    int counting_array[maximum_value + 1];
    for (int i = 0; i < maximum_value + 1; i++)
        counting_array[i] = 0;

    // increase the counting array
    for (int i = 0; i < n; i++)
        counting_array[values[i]] += 1;

    // make the array sorted
    int counter = 0;
    for (int i = 0; i < maximum_value + 1; i++)
    {
        if (counting_array[i] != 0 && counter < n)
        {
            while (counting_array[i] != 0)
            {
                values[counter] = i;
                counting_array[i] -= 1;
                counter += 1;
            }

        }
    }


    return;
}
