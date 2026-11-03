/* is_palindrome_u128: whether n is a decimal palindrome.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_PALINDROME_U128_H
#define OEISC_UTILS_IS_PALINDROME_U128_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Test whether an unsigned __int128 number is a decimal palindrome. */
static bool is_palindrome_u128(unsigned __int128 v)
{
    unsigned __int128 rev = 0, orig = v;
    if (v == 0) return true;
    while (v > 0) {
        rev = rev * 10 + v % 10;
        v /= 10;
    }
    return rev == orig;
}

#endif
