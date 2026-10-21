/*
 * A263527  Expansion of phi(-x^3) * f(-x^6)^3 / f(-x^2) in powers of x where
 *          phi(), f() are Ramanujan theta functions.
 * https://oeis.org/A263527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the Euler transform of the period‑6 sequence * [0, 1,
 *            -2, 1, 0, -3, …] given by * c(k) = Σ_{d|k} d * b(d), where b(d)
 *            = 0,1,-2,1,0,-3 (period 6) * and the recurrence * n * a(n) =
 *            Σ_{k=1}^{n} c(k) * a(n-k), a(0) = 1 * This reproduces the
 *            coefficients of * phi(-x³) * f(-x⁶)³ / f(-x²)
 * Range:     i8 n <= 23, i16 n <= 811, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_sz = 0;

    /* treat trivial case */
    if (n == 0)
        return (seq_t)1;

    /* if we don't have enough precomputed values, extend the cache */
    if (cache == NULL || (size_t)n >= cache_sz)
    {
        /* size needed */
        size_t new_sz = (size_t)n + 1u;

        /* Allocate temporary array for c(k) as 64‑bit signed integers */
        long long *c = (long long *)calloc(new_sz, sizeof(long long));
        if (c == NULL)
            abort();   /* allocation failure */

        /* period‑6 values for b(d) */
        const int period[6] = {0, 1, -2, 1, 0, -3};

        /* compute c(k) = Σ_{d|k} d * b(d) */
        for (size_t d = 1; d < new_sz; ++d)
        {
            int bd = period[(d - 1u) % 6u];
            if (bd == 0)
                continue;
            long long inc = (long long)d * bd;
            for (size_t m = d; m < new_sz; m += d)
                c[m] += inc;
        }

        /* Reallocate cache to new size */
        seq_t *new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (new_cache == NULL)
        {
            free(c);
            abort();
        }
        cache   = new_cache;
        cache_sz = new_sz;

        /* initialise a(0) */
        cache[0] = (seq_t)1;

        /* compute a(k) for k = 1 .. n using the recurrence */
        for (size_t i = 1; i < new_sz; ++i)
        {
            seq_t sum = (seq_t)0;
            for (size_t k = 1; k <= i; ++k)
                sum += ((seq_t)c[k]) * cache[i - k];
            cache[i] = sum / (seq_t)i;   /* integer division, result fits in seq_t */
        }

        free(c);
    }

    return cache[(size_t)n];
}
