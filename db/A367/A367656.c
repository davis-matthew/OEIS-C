/*
 * A367656  G.f. A(x) satisfies A(x) = 1 / (1 - x - x * (1 + x + x^2) *
 *          A(x^3)).
 * https://oeis.org/A367656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = a(n-1) + Σ_{k=0}^{n-1}
 *            a(floor(k/3)) * a(n-1-k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th term of OEIS A367656.  Memoises previously computed terms
 * in a dynamically resized static array so that calls can be made in any order.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static idx_t next = 0;   /* next uninitialised index in the cache */

    /* initialise for first call */
    if (!cache)
    {
        /* start with a modest capacity, grow only when needed */
        cache_capacity = (size_t)(n + 1 > 16 ? n + 1 : 16);
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;                  /* a(0) = 1 */
        next = 1;
    }

    /* enlarge cache if required */
    if ((size_t)n + 1 > cache_capacity)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed, keep old cache */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* compute missing values up to n */
    while (next <= n)
    {
        idx_t i = next;
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k)
        {
            idx_t f = k / 3;                /* floor(k/3) */
            sum += cache[f] * cache[i - 1 - k];
        }
        cache[i] = cache[i - 1] + sum;      /* a(i) = a(i-1) + sum */
        ++next;
    }

    return cache[n];
}
