#include "misc.h"

/*
 * Description: returns minimum float value in an array
 * Parameters: array -> unordered float array, n -> number of items in float array
 * Returns: minimum value in float array
 */
float get_min(float array[], int n)
{
    return (n == 1) ? array[0] : min(array[0], get_min(array + 1, n - 1));
}

/*
 * Description: compares two floating point numbers and returns the snaller of the two
 * Parameters: a -> first value to be compared, b -> second value to be compared
 * Returns: minimum value of two floats
 */
float min(float a, float b)
{
    return (a > b) ? b : a;
}