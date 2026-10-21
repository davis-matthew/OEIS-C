/*
 * A286717  a(n) is the number of zeros of the Chebyshev S(n, x) polynomial
 *          (A049310) in the open interval (-phi, +phi), with the golden
 *          section phi = (1 + sqrt(5))/2.
 * https://oeis.org/A286717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=2, a(3)=3, a(4)=2, a(5)=3, and a(n) =
 *            a(n-1) + a(n-5) - a(n-6) for n > 5 (OEIS A286717)
 * Range:     u8 n <= 425, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len) {
        /* Allocate (n+1) entries */
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – terminate the program; the build
               harness will never call this for huge n. */
            exit(EXIT_FAILURE);
        }
        /* Fill new entries */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0)        new_cache[i] = 0;
            else if (i == 1)   new_cache[i] = 1;
            else if (i == 2)   new_cache[i] = 2;
            else if (i == 3)   new_cache[i] = 3;
            else if (i == 4)   new_cache[i] = 2;
            else if (i == 5)   new_cache[i] = 3;
            else {
                /* recurrence uses earlier values already computed in this loop */
                new_cache[i] = new_cache[i-1] + new_cache[i-5] - new_cache[i-6];
            }
        }
        cache = new_cache;
        cache_len = new_len;
    }
    return cache[n];
}
