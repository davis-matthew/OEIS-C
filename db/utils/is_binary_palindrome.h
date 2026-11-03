/* is_binary_palindrome: whether n is a binary palindrome.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_BINARY_PALINDROME_H
#define OEISC_UTILS_IS_BINARY_PALINDROME_H
#include <stdbool.h>
#include <stdint.h>

/* Return true iff x is a palindrome in base 2. */
static bool is_binary_palindrome(seq_t x)
{
    seq_t rev = 0;
    seq_t tmp = x;
    while (tmp) {
        rev = (rev << 1) | (tmp & 1);
        tmp >>= 1;
    }
    return rev == x;
}

#endif
