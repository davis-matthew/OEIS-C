/*
 * A384732  Expansion of (1+2*x) / (1-x-6*x^2+2*x^3).
 * https://oeis.org/A384732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 6*a(n-2) - 2*a(n-3),
   a(0) = 1, a(1) = 3, a(2) = 9
   (OEIS A384732) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap  = 0;   /* allocated size */
    static size_t len  = 0;   /* number of terms computed */

    /* initialise cache on first call */
    if (cap == 0)
    {
        cap  = 3;                      /* store a(0), a(1), a(2) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely */
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 9;
        len = 3;
    }

    /* already computed? */
    if ((size_t)n < len) return cache[n];

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cap)
    {
        size_t new_cap = cap * 2;
        if (new_cap < cap + (size_t)(n + 1))
            new_cap = cap + (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – unlikely */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute all missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i)
    {
        cache[i] = cache[i-1] + 6 * cache[i-2] - 2 * cache[i-3];
    }

    len = (size_t)n + 1;
    return cache[n];
}
