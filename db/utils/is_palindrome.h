/* is_palindrome: whether n is a decimal palindrome.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_PALINDROME_H
#define OEISC_UTILS_IS_PALINDROME_H
#include <stdbool.h>
#include <stdint.h>

/* helper: test whether a seq_t value is a decimal palindrome */
static bool is_palindrome(seq_t x)
{
    seq_t orig = x;
    seq_t rev  = 0;
    while (x > 0)
    {
        rev = rev * 10 + x % 10;
        x   = x / 10;
    }
    return rev == orig;
}

#endif
