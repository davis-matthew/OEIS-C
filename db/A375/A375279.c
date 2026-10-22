/*
 * A375279  Expansion of (1 - x - x^3)/((1 - x - x^3)^2 - 4*x^4).
 * https://oeis.org/A375279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) + 2*a(n-4) - a(n-6) Initial
 *            values: a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=7, a(5)=16
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoized values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* Return cached value if already available */
    if (n < cache_cap)
        return cache[n];

    /* remember old capacity before resizing */
    idx_t old_cap = cache_cap;

    /* Grow array to size n+1 */
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                     /* allocation failure */
        return 0;
    cache = tmp;
    cache_cap = new_cap;

    /* Initial values for the first six indices */
    const seq_t init[6] = { (seq_t)1, (seq_t)1, (seq_t)1,
                            (seq_t)2, (seq_t)7, (seq_t)16 };

    /* Fill any missing initial entries */
    for (idx_t i = old_cap; i < 6 && i < new_cap; ++i)
        cache[i] = init[i];

    /* Compute remaining terms using the recurrence relation */
    idx_t start = old_cap > 6 ? old_cap : 6;
    for (idx_t i = start; i < new_cap; ++i) {
        cache[i] = ((seq_t)2 * cache[i-1])          /* 2*a(n-1) */
                  -                 cache[i-2]       /* - a(n-2) */
                  + ((seq_t)2 * cache[i-3])         /* + 2*a(n-3) */
                  + ((seq_t)2 * cache[i-4])         /* + 2*a(n-4) */
                  -                 cache[i-6];      /* - a(n-6) */
    }

    return cache[n];
}
