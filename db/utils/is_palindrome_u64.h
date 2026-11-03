/* is_palindrome_u64: whether n is a decimal palindrome.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_PALINDROME_U64_H
#define OEISC_UTILS_IS_PALINDROME_U64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Return true if x is a palindrome in base 10. */
static bool is_palindrome_u64(unsigned long long x)
{
    unsigned long long rev = 0, tmp = x;
    while (tmp > 0) {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
    }
    return rev == x;
}

#endif
