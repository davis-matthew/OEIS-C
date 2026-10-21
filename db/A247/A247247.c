/*
 * A247247  Triangular numbers that are the sum of 2 consecutive terms of
 *          A130518.
 * https://oeis.org/A247247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A247247
 * a(1) = 0, a(2) = 1, a(3) = 3, a(4) = 21, a(5) = 120, a(6) = 300,
 * a(7) = 2080, a(8) = 11781, a(9) = 29403
 *
 * The sequence satisfies the linear recurrence
 *     a(n) = 99·a(n‑3) – 99·a(n‑6) + a(n‑9)      for n ≥ 10
 * (coefficients obtained from the empirical generating function
 *  x²(x+1)(x⁴+2x³+19x²+2x+1)/((1–x)(x²+x+1)(x⁶–98x³+1)).)
 * The function memoises computed terms to answer calls in any order.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset.  Behavior is irrelevant. */
    if (n == (idx_t)0)  return (seq_t)0;

    /* Static cache: cache[1] … cache[k] hold a(1)…a(k). */
    static seq_t *cache   = NULL;
    static idx_t  cache_n = 0;   /* highest index that is stored */

    /* Initial terms up to a(9). */
    static const seq_t init[9] =
    {   0, 1, 3, 21, 120, 300, 2080, 11781, 29403 };

    /* Initialise cache on the first call. */
    if (cache == NULL)
    {
        /* Allocate space for indices 0..9 (index 0 is unused). */
        cache_n = 9;
        cache   = (seq_t *)calloc((size_t)(cache_n + 1), sizeof(seq_t));
        if (cache == NULL)  return (seq_t)0;   /* allocation failed */

        /* Fill the first nine values. */
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = init[(size_t)(i - 1)];
    }

    /* Return directly for known values. */
    if (n <= cache_n)  return cache[(size_t)n];

    /* Expand the cache up to n. */
    if (n > cache_n)
    {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)(n + 1) * sizeof(seq_t));
        if (new_cache == NULL)  /* allocation failed */
            return (seq_t)0;
        cache = new_cache;

        /* Compute terms from cache_n+1 up to n using the recurrence. */
        for (idx_t i = cache_n + 1; i <= n; ++i)
        {
            cache[(size_t)i] =
                (seq_t)99 * cache[(size_t)(i - 3)]
              - (seq_t)99 * cache[(size_t)(i - 6)]
              +          cache[(size_t)(i - 9)];
        }
        cache_n = n;   /* update highest stored index */
    }

    return cache[(size_t)n];
}
