/*
 * A350924  a(0) = 1, a(1) = 3, and a(n) = 16*a(n-1) - a(n-2) - 4 for n >= 2.
 * https://oeis.org/A350924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - a(n-2) - 4, a(0) = 1, a(1) = 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated size of cache */
    static idx_t max_computed = 0;    /* highest n that has been computed */

    /* Handle base cases directly */
    if (n == 0) return 1;
    if (n == 1) return 3;

    /* Make sure cache can hold up to n */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
    }

    if (n >= cap) {
        /* grow cache exponentially until it fits n */
        while (cap <= n) {
            size_t new_cap = cap * 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }
    }

    /* Initialize cache for the very first two terms if not already done */
    if (max_computed < 1) {
        cache[0] = 1;
        cache[1] = 3;
        max_computed = 1;
    }

    /* Compute subsequent terms up to n using recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = 16 * cache[i - 1] - cache[i - 2] - 4;
    }

    max_computed = n;

    return cache[n];
}
