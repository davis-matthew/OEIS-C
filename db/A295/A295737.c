/*
 * A295737  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 1,
 *          a(1) = -1, a(2) = -1, a(3) = 2.
 * https://oeis.org/A295737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4); initial values:
 *            a(0)=1, a(1)=-1, a(2)=-1, a(3)=2
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 43, i64 n <= 89, i128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External function to compute the nth term of OEIS sequence A295737 */
seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t  max_computed = 0;   /* largest n for which cache is valid */
    static size_t cache_cap = 0;      /* allocated capacity of cache */

    /* Helper to grow and compute missing terms up to n */
    /* This function may be called multiple times with arbitrary n. */
    if (!cache) {
        size_t init_cap = (n < 4 ? 4 : n + 1);
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-1;
        cache[3] = (seq_t)2;
        max_computed = 3;
    }

    if (n > max_computed) {
        /* Extend cache if necessary */
        if (n >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= n) new_cap = n + 1;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            for (idx_t i = 0; i <= max_computed; ++i)
                new_cache[i] = cache[i];
            free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }
        /* Compute missing terms using the recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = cache[i-1] + 3*cache[i-2] - 2*cache[i-3] - 2*cache[i-4];
        }
        max_computed = n;
    }

    return cache[n];
}
