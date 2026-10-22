/*
 * A385002  Consecutive states of the linear congruential pseudo-random number
 *          generator (211*s + 1663) mod 7875 when started at s=1.
 * https://oeis.org/A385002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = (211 * a(n-1) + 1663) mod 7875 for n > 1, a(1) = 1 This
 *            function memoizes previously computed terms to support arbitrary
 *            call order
 * Range:     u8 n <= 1, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* 1‑based array of computed terms */
    static size_t cache_cap = 0;      /* allocated capacity (size_t) */
    static size_t cache_size = 0;     /* number of terms already computed */

    if (n < 1)
        return (seq_t)0;              /* undefined for offset 1, but safe fallback */

    /* Ensure the cache can hold n terms. */
    if (cache_cap < (size_t)(n + 1))
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
        {
            /* allocation failed – return 0 as a safe error indicator */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the required term is already computed, return it. */
    if ((size_t)n <= cache_size)
        return cache[(size_t)n];

    /* Compute missing terms up to n, starting from the 1st term if not yet present. */
    if (cache_size == 0)
    {
        cache[1] = (seq_t)1;
        cache_size = 1;
    }

    for (size_t i = cache_size + 1; i <= (size_t)n; ++i)
    {
        seq_t prev = cache[i - 1];
        cache[i] = ((prev * (seq_t)211) + (seq_t)1663) % (seq_t)7875;
    }

    cache_size = (size_t)n;
    return cache[n];
}
