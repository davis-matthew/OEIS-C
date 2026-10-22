/*
 * A394478  a(n) = (2*n)! * [x^(2*n)] cosh(x)^2 / cos(x).
 * https://oeis.org/A394478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (0^n + 4^n)/2 - Σ_{k=0}^{n-1} (-1)^{n-k} *
 *            binomial(2n,2k) * a(k), with a(0) = 1. This implementation
 *            memoises the values up to the required index
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

static seq_t *a_cache = NULL;        /* memoisation array */
static idx_t  cache_cap = 0;         /* current allocated capacity */
static idx_t  cache_used = 0;        /* number of entries really computed */

/* Ensure values up to index n are computed and cached. */
static void
ensure_cached(idx_t n)
{
    if (cache_used > n)
        return;                      /* already cached */

    /* Grow allocation if necessary. */
    if (cache_cap <= n) {
        size_t new_cap = (size_t)cache_cap > 0 ? 2 * cache_cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms from cache_used up to n. */
    for (idx_t i = cache_used; i <= n; ++i) {
        if (i == 0) {                /* base case */
            a_cache[0] = (seq_t)1;
            continue;
        }

        /* (0^i + 4^i)/2 = 4^i/2 = 2^(2*i-1) for i>0 */
        seq_t t0 = pow2(2 * i - 1);

        seq_t sum = 0;               /* Σ (-1)^{i-k} * C(2i,2k) * a(k)   */
        seq_t binom = (seq_t)1;       /* C(2i,0) */

        for (idx_t k = 0; k < i; ++k) {
            int sign = ((i - k) & 1) ? -1 : 1;  /* (-1)^{i-k} */

            seq_t term = binom * a_cache[k];
            if (sign >= 0)
                sum += term;
            else
                sum -= term;

            /* Update binom to C(2i,2(k+1)) for next iteration. */
            if (k + 1 < i) {
                seq_t num = (seq_t)(2 * (i - k)) * (seq_t)(2 * (i - k) - 1);
                seq_t den = (seq_t)(2 * k + 1) * (seq_t)(2 * k + 2);
                binom = (binom * num) / den;
            }
        }

        a_cache[i] = t0 - sum;
    }

    cache_used = n + 1;
}

/* Public API: compute a(n) */
seq_t a(idx_t n)
{
    ensure_cached(n);
    return a_cache[n];
}
