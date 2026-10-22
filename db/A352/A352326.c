/*
 * A352326  Expansion of e.g.f.: 1/(2 - exp(x) - sinh(x)).
 * https://oeis.org/A352326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..n} (3-(-1)^k)/2 * binomial(n,k) *
 *            a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A352326 */
#include <stdlib.h>

/* a(n): compute and memoize sequence values */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements stored */

    if (cache == NULL) {
        /* initialise cache with a(0) */
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure – unlikely         */
        cache[0] = (seq_t)1;
    }

    if (n < cache_size)
        return cache[n];

    /* expand cache up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;          /* allocation failure */
    cache = tmp;

    /* compute missing values */
    for (idx_t m = cache_size; m <= n; ++m) {
        if (m == 0) {            /* a(0) already set, but keep for clarity */
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t binC = (seq_t)1;   /* binomial(m,0) */

        for (idx_t k = 1; k <= m; ++k) {
            /* binomial(m,k) = binomial(m,k-1) * (m-k+1) / k */
            binC = binC * (seq_t)(m - k + 1) / (seq_t)k;

            /* coefficient: 2 for odd k, 1 for even k */
            seq_t coeff = (k & (idx_t)1) ? (seq_t)2 : (seq_t)1;

            /* term = coeff * binomial(m,k) * a(m-k) */
            seq_t term = coeff * binC;
            term *= cache[m - k];

            sum += term;
        }

        cache[m] = sum;
    }

    cache_size = new_size;
    return cache[n];
}
