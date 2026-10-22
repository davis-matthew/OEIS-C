/*
 * A340977  Number of ways to write n as an ordered sum of 4 positive cubes.
 * https://oeis.org/A340977
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{s=0}^{n} f(s)·f(n–s) where f(t) = number of ordered
 *            pairs (i,j) of positive cubes with i^3 + j^3 = t
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static cache for f(t) and a(n). */
static seq_t *f_cache   = NULL;   /* f(t) values */
static seq_t *a_cache   = NULL;   /* a(n) values */
static idx_t  max_n     = 0;      /* largest n that has been fully computed */

/* Compute floor of integer cube root of x */
static idx_t floor_cube_root(idx_t x)
{
    idx_t k = 0;
    while ((k + 1) * (k + 1) * (k + 1) <= x)
        ++k;
    return k;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    /* Grow caches if necessary */
    if (n > max_n)
    {
        /* Extend f_cache */
        if (f_cache == NULL || (idx_t)(max_n + 1) < n + 1)
        {
            seq_t *new_f = (seq_t *)realloc(f_cache, (size_t)(n + 1) * sizeof(seq_t));
            if (new_f == NULL) abort();
            f_cache = new_f;
        }
        /* Extend a_cache */
        if (a_cache == NULL || (idx_t)(max_n + 1) < n + 1)
        {
            seq_t *new_a = (seq_t *)realloc(a_cache, (size_t)(n + 1) * sizeof(seq_t));
            if (new_a == NULL) abort();
            a_cache = new_a;
        }

        /* Re‑compute f(t) for 0 ≤ t ≤ n */
        memset(f_cache, 0, (size_t)(n + 1) * sizeof(seq_t));
        idx_t K = floor_cube_root(n);
        for (idx_t i = 1; i <= K; ++i)
        {
            idx_t ci = i * i * i;
            for (idx_t j = 1; j <= K; ++j)
            {
                idx_t cj = j * j * j;
                idx_t sum = ci + cj;
                if (sum > n)
                    break;
                f_cache[sum] += 1;
            }
        }

        /* Compute a(n) via convolution of f with itself */
        memset(a_cache, 0, (size_t)(n + 1) * sizeof(seq_t));
        for (idx_t t = 0; t <= n; ++t)
        {
            seq_t ft = f_cache[t];
            if (ft == 0)
                continue;
            for (idx_t u = 0; u <= n - t; ++u)
            {
                seq_t fu = f_cache[u];
                if (fu == 0)
                    continue;
                a_cache[t + u] += ft * fu;
            }
        }

        max_n = n;
    }

    return a_cache[n];
}
