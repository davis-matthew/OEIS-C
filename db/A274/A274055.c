/*
 * A274055  Relative of Hofstadter Q-sequence: a(n) = n for 1 <= n <= 42; a(n)
 *          = a(n-a(n-1)) + a(n-a(n-2)) for n > 42.
 * https://oeis.org/A274055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for 1 <= n <= 42; a(n) = a(n-a(n-1)) + a(n-a(n-2)) for
 *            n > 42, with a(n) = 0 for n <= 0
 * Range:     u8 n <= 86, u16 n <= 407, u32 n <= 9195, u64 n <= 9195,
 *            u128 n <= 9195, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;   /* Table of already computed values */
    static idx_t  cap   = 0;      /* Current capacity of the table */
    static idx_t  computed = 0;   /* Highest index for which cache is valid */

    /* Grow the table if needed */
    if (n >= cap) {
        idx_t newcap = cap ? cap : 64;
        while (newcap <= n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise the table for the first elements */
    if (computed == 0) {
        cache[0] = (seq_t)0;
        for (idx_t i = 1; i <= 42 && i < cap; ++i)
            cache[i] = (seq_t)i;
        computed = 42;
    }

    /* Compute up to the requested index if necessary */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i <= 42) {
                cache[i] = (seq_t)i;
            } else {
                seq_t prev1 = cache[i - 1];
                seq_t prev2 = cache[i - 2];

                idx_t idx1;
                if (prev1 > (seq_t)i)
                    idx1 = 0;
                else
                    idx1 = i - (idx_t)prev1;

                idx_t idx2;
                if (prev2 > (seq_t)i)
                    idx2 = 0;
                else
                    idx2 = i - (idx_t)prev2;

                cache[i] = cache[idx1] + cache[idx2];
            }
        }
        computed = n;
    }

    return cache[n];
}
