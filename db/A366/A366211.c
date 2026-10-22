/*
 * A366211  Numbers whose median of the digits is equal to 5.
 * https://oeis.org/A366211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   median_is_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose median of the decimal digits is equal to 5
   (OEIS A366211).  This implementation searches for the
   n‑th number whose median digit equals 5, storing results
   in a static cache so that calls in any order are fast. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/median_is_five.h"

/* A large enough cache to hold thousands of terms. */
#define CACHE_SIZE 500000
static seq_t cache[CACHE_SIZE];
static size_t len = 0;   /* number of terms already found */
static seq_t cur = 5;    /* next integer to test for the median property */

/* Return the n-th number whose median of the decimal digits equals 5. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, but guard anyway */

    /* Generate terms until we have at least n of them. */
    while (len < (size_t)n) {
        if (median_is_five(cur))
            cache[len++] = cur;
        cur++;
    }
    return cache[(size_t)n - 1];
}
