/*
 * A346053  G.f. A(x) satisfies: A(x) = 1 - x * A(x/(1 - x)) / (1 - x)^3.
 * https://oeis.org/A346053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 16, i64 n <= 27, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A346053
   Recurrence:  a(0) = 1
                a(n+1) = -∑_{k=0..n} binomial(n+2, k+2) * a(k)   (n ≥ 0)
*/
#include <stdlib.h>   /* for malloc, free */

static seq_t *cache = NULL;   /* cache of computed terms */
static idx_t cache_len = 0;   /* number of cached terms */

/* extend the cache to contain terms up to n (inclusive) */
static void extend_cache(idx_t n)
{
    if (cache_len > n) return;              /* already big enough */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
    for (idx_t i = 0; i < cache_len; ++i)   /* copy old values */
        new_cache[i] = cache[i];
    if (cache != NULL) free(cache);         /* free old memory */
    cache = new_cache;
    cache_len = new_len;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* base case a(0) = 1 */
    if (n == 0) {
        if (cache_len < 1) {
            extend_cache(0);
            cache[0] = 1;
            cache_len = 1;
        }
        return cache[0];
    }

    /* ensure cache contains terms up to n */
    if (cache_len <= n) {
        idx_t old_len = cache_len;  /* value before extension */
        extend_cache(n);
        /* compute missing terms starting from old_len */
        for (idx_t m = old_len; m <= n; ++m) {
            /* a(m) = -∑_{k=0..m-1} binomial(m+1, k+2) * a(k) */
            seq_t sum = 0;
            /* start with C(m+1, 2) */
            seq_t c = ((seq_t)(m + 1) * (seq_t)m) / 2;
            for (idx_t k = 0; k < m; ++k) {
                sum += c * cache[k];
                if (k + 1 < m) {
                    /* update c to C(m+1, k+3) */
                    seq_t num = (seq_t)(m - k - 1);   /* N - r where N=m+1, r=k+2 */
                    seq_t den = (seq_t)(k + 3);        /* r+1 */
                    c = (c * num) / den;
                }
            }
            cache[m] = -sum;
        }
    }

    return cache[n];
}
