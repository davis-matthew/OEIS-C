/*
 * A376648  a(n) = Sum_{k=0..floor(n/4)} binomial(floor(k/2),n-4*k).
 * https://oeis.org/A376648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A376648
   a(n) = Σ_{k=0..⌊n/4⌋} binomial(⌊k/2⌋, n-4k)
   Equivalent recurrence: a(n) = a(n-8) + a(n-9)
   Initial values (offset 0): a(0)=1, a(1)=a(2)=a(3)=a(5)=a(6)=a(7)=0, a(4)=1
   (All other a(n) for n≥8 follow the recurrence.) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* memoization array */
    static size_t  cap   = 0;      /* current number of cached entries */

    size_t i = (size_t)n;

    if (i >= cap) {
        /* Grow the cache to include index i */
        size_t new_cap = i + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        cache = tmp;

        /* Initialize new entries */
        for (size_t j = cap; j < new_cap; ++j) {
            if (j < 8) {
                /* base values */
                if (j == 0 || j == 4)
                    cache[j] = (seq_t)1;
                else
                    cache[j] = (seq_t)0;
            } else {
                /* recurrence a(j) = a(j-8) + a(j-9) */
                seq_t v1 = cache[j - 8];
                seq_t v2 = (j >= 9) ? cache[j - 9] : (seq_t)0;
                cache[j] = v1 + v2;
            }
        }
        cap = new_cap;
    }

    return cache[i];
}
