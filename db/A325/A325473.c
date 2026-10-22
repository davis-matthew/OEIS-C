/*
 * A325473  Number of compositions of n with no part divisible by 3 and an
 *          even number of parts congruent to 4 or 5 modulo 6.
 * https://oeis.org/A325473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A325473.
 * Recurrence (for n ≥ 6):
 *    a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + a(n-5)
 * Initial values:
 *    a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=8
 * The sequence satisfies the rational generating function
 *    (x^5-x^4+x^3-x^2+2x-1)/((x^3+x^2+x-1)*(x-1)^2).
 * This implementation uses memoisation with a dynamic array.
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: a_cache[i] = a(i) for 0 <= i < cache_len */
    static seq_t *a_cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache with first six values */
    if (cache_len == 0) {
        cache_len = 6;
        a_cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        a_cache[0] = 1;
        a_cache[1] = 1;
        a_cache[2] = 2;
        a_cache[3] = 3;
        a_cache[4] = 5;
        a_cache[5] = 8;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        /* compute values from cache_len to n (inclusive) */
        for (size_t i = cache_len; i <= n; ++i) {
            if (i < 6) {
                /* i < 6 values are already initialised */
                continue;
            }
            /* a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + a(n-5)
             * prevent unsigned underflow by grouping positive terms:
             * result = (3*a(n-1) + a(n-5)) - (2*a(n-2) + a(n-4))
             */
            seq_t term1 = 3 * a_cache[i - 1] + a_cache[i - 5];
            seq_t term2 = 2 * a_cache[i - 2] + a_cache[i - 4];
            a_cache[i] = term1 - term2;
        }
        cache_len = new_len;
    }

    return a_cache[(size_t)n];
}
