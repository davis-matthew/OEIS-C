/*
 * A346984  Expansion of e.g.f. 1 / (6 - 5 * exp(x))^(1/5).
 * https://oeis.org/A346984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = a(n-1) - 6 * Σ_{k=1}^{n-1} (-1)^k *
 *            binomial(n-1,k) * a(n-k) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
/* seq_t and idx_t are supplied by the build */

seq_t a(idx_t n)
{
    /* maximum n expected (≈ 10000) */
    const idx_t MAX_N = 10000;

    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;     /* current capacity */
    static idx_t next_compute = 0;    /* next index to compute (cache[0..next_compute-1] ready) */

    /* ensure cache is large enough to hold a[n] */
    if (n >= cache_size) {
        size_t new_size = (n < MAX_N ? MAX_N : n) + 1;
        seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (cache) {
            /* copy existing values */
            memcpy(new_cache, cache, (next_compute > cache_size ? cache_size : next_compute) * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* initialise first element if needed */
    if (next_compute == 0) {
        cache[0] = (seq_t)1;
        next_compute = 1;
    }

    /* compute values up to n */
    while (next_compute <= n) {
        idx_t m = next_compute;          /* compute a[m] */
        seq_t sum = 0;                   /* Σ_{k=1}^{m-1} (-1)^k * binom(m-1,k) * a[m-k] */
        seq_t binom = (seq_t)1;           /* starting with C(m-1,0) */

        for (idx_t k = 1; k < m; ++k) {
            /* update binom to C(m-1, k) */
            binom = (binom * (seq_t)(m - k)) / (seq_t)k;

            seq_t term = binom * cache[m - k];
            if (k & 1U)            /* odd k -> negative sign */
                sum -= term;
            else                    /* even k -> positive sign */
                sum += term;
        }

        /* a[m] = a[m-1] - 6 * sum */
        cache[m] = cache[m - 1] - (seq_t)6 * sum;
        ++next_compute;
    }

    return cache[n];
}
