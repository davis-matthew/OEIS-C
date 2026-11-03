/* is_palindrome_idx: whether n is a decimal palindrome.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_PALINDROME_IDX_H
#define OEISC_UTILS_IS_PALINDROME_IDX_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: check whether an unsigned integer is a decimal palindrome. */
static bool is_palindrome_idx(idx_t x)
{
    idx_t rev = 0, tmp = x;
    while (tmp > 0) {
        rev = rev * 10 + tmp % 10;
        tmp /= 10;
    }
    return rev == x;
}

#endif
