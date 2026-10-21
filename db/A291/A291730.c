/*
 * A291730  p-INVERT of (1,0,1,0,0,0,0,...), where p(S) = 1 - 2 S - 2 S^2.
 * https://oeis.org/A291730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=6, a(2)=18, a(3)=56, a(4)=168, a(5)=510, a(6)=1544
   For n >= 7: a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) + 4*a(n-4) + 2*a(n-6) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if ((size_t)n >= cache_cap) {
        /* Grow cache to hold indices up to n */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = NULL;

        if (cache == NULL) {
            tmp = (seq_t *)calloc(new_cap, sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;

        /* Initialize the first seven terms */
        static const seq_t init[7] = {2, 6, 18, 56, 168, 510, 1544};
        for (size_t i = 0; i < 7 && i < new_cap; ++i)
            cache[i] = init[i];

        /* Compute remaining terms using the recurrence. */
        size_t start = (cache_cap <= 7) ? 7 : cache_cap;
        for (size_t i = start; i < new_cap; ++i) {
            cache[i] = 2 * cache[i-1]
                     + 2 * cache[i-2]
                     + 2 * cache[i-3]
                     + 4 * cache[i-4]
                     + 2 * cache[i-6];
        }

        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
