/*
 * A386510  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} (1 + k) * binomial(k+1,2)
 *          * binomial(n-1,k) * a(k) * a(n-1-k).
 * https://oeis.org/A386510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + Σ_{k=0}^{n-1} (1 + k) *
 *            binomial(k+1,2) * binomial(n-1,k) * a(k) * a(n-1-k) (OEIS
 *            notation)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* memoization array */
static idx_t cache_len = 0;          /* number of terms computed (next free index) */
static idx_t cache_cap = 0;          /* allocated capacity */

/* ensure the cache can hold up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;

    idx_t new_cap = cache_cap ?: 1;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* insufficient memory */

    /* initialize newly allocated slots to zero */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    ensure_cache(n);

    /* compute all missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }

        idx_t m = i - 1;                 /* row index for binomial coefficients */
        seq_t sum = 0;
        seq_t bin = 1;                    /* binomial(m,0) */

        for (idx_t k = 0; k <= m; ++k)
        {
            /* w_k = (k+1)^2 * k / 2 */
            seq_t w = (seq_t)(k + 1);
            w *= w;                       /* (k+1)^2 */
            seq_t t = (seq_t)k;
            w *= t;                       /* (k+1)^2 * k */
            w /= 2;                       /* exact division */

            seq_t term = bin;             /* binomial(m,k) */
            term *= cache[k];             /* a(k) */
            term *= cache[m - k];         /* a(m-k) */
            term *= w;                    /* weighted by w_k */

            sum += term;

            /* update bin to binomial(m,k+1) if needed */
            if (k < m) {
                seq_t num = (seq_t)(m - k);   /* numerator (m-k) */
                seq_t den = (seq_t)(k + 1);   /* denominator (k+1) */
                bin *= num;
                bin /= den;                   /* exact division */
            }
        }

        cache[i] = cache[i - 1] + sum;
    }

    cache_len = n + 1;                    /* all up to n computed */
    return cache[n];
}
