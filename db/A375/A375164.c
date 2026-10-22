/*
 * A375164  Expansion of 1/((1 - x)^4 - 2*x^4).
 * https://oeis.org/A375164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) + a(n-4) (initial values:
 *            a(0)=1, a(1)=4, a(2)=10, a(3)=20)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for the first four terms */
    static const seq_t init[4] = { (seq_t)1u, (seq_t)4u, (seq_t)10u, (seq_t)20u };

    /* Cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of valid entries in cache */

    /* If n is within the initial range, return directly. */
    if (n < 4)
        return init[(size_t)n];

    /* Ensure the cache can hold at least the first four terms. */
    if (cache_size < 4)
    {
        size_t sz = 4;
        seq_t *tmp = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        tmp[0] = init[0]; tmp[1] = init[1];
        tmp[2] = init[2]; tmp[3] = init[3];
        cache = tmp;
        cache_size = sz;
    }

    /* Extend the cache if the requested index is not yet computed. */
    if (n >= cache_size)
    {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();            /* reallocation failed */
        cache = tmp;

        /* Compute missing terms using the recurrence. */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            /* Compute: 4*a[i-1] - 6*a[i-2] + 4*a[i-3] + a[i-4]  */
            /* Use the order 4*a[i-1] + 4*a[i-3] + a[i-4] - 6*a[i-2]  */
            seq_t term = cache[i - 1] << 2;      /* 4*a[i-1]   */
            term += cache[i - 3] << 2;            /* +4*a[i-3]  */
            term += cache[i - 4];                 /* +a[i-4]    */
            term -= cache[i - 2] * (seq_t)6u;      /* -6*a[i-2]  */

            cache[i] = term;
        }

        cache_size = new_sz;
    }

    return cache[(size_t)n];
}
