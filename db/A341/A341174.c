/*
 * A341174  Numbers that when divided by the sum of their digits leave 7 as
 *          remainder.
 * https://oeis.org/A341174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 2173, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search:
   a(n) = the nth positive integer x such that x % sum_of_digits(x) == 7.
   The offset is 1. */
#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /*  Static state to cache already computed terms.  */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  count = 0;
    static seq_t next   = 1;   /* next candidate to test */

    /*  Allocate the cache on the first call.  */
    if (cap == 0) {
        cap   = 64;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
    }

    /*  Generate terms until we have at least n of them.  */
    while (count < n) {
        seq_t sum = digit_sum(next);          /* sum of decimal digits */
        if (sum != (seq_t)0 && next % sum == (seq_t)7) {
            /*  Grow the array if needed.  */
            if (count == cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[(size_t)count++] = next;
        }
        ++next;
    }

    return cache[(size_t)(n - 1)];
}
