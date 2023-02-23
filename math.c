#include <stdint.h>

// https://www.geeksforgeeks.org/write-a-c-program-to-calculate-powxn/
int power(int x, int n)
{
    int result = 1;
    while (n > 0) {
        if (n & 1 == 1)
        {
            result = result * x;
        }
        x = x * x;
        n = n >> 1;
    }
    return result;
}

int right_rotate(uint8_t n, uint8_t d)
{
    return (n >> d)|(n << (8 - d));
}

// https://www.geeksforgeeks.org/square-root-of-an-integer/
uint8_t floor_sqrt(int x)
{
    // Base cases
    if (x == 0 || x == 1)
        return x;
 
    // Starting from 1, try all numbers until
    // i*i is greater than or equal to x.
    int i = 1, result = 1;
    while (result <= x) {
        i++;
        result = i * i;
    }
    return i - 1;
}


