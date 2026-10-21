/*
 * A292027  a(n) = a(n-7) + a(n-11), starting a(0)=a(1)=...= a(10) = 1.
 * https://oeis.org/A292027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-7) + a(n-11), a(0..10)=1
 * Range:     u8 n <= 75, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* memoization array */
    static size_t cap   = 0;      /* allocated capacity (number of slots) */
    static size_t len   = 0;      /* number of terms currently stored */

    /* Ensure we have computed up to n inclusive. */
    if ((size_t)n >= len) {
        /* Grow the array if necessary.  Double until we can hold n+1. */
        size_t required = (size_t)n + 1;
        if (cap < required) {
            size_t new_cap = cap ? cap * 2 : 16;
            while (new_cap < required) {
                new_cap <<= 1;
            }
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        /* Compute missing terms. */
        for (size_t i = len; i < required; ++i) {
            if (i <= 10) {
                cache[i] = (seq_t)1;
            } else {
                cache[i] = cache[i - 7] + cache[i - 11];
            }
        }
        len = required;
    }

    return cache[(size_t)n];
}
