/*
 * A308862  Expansion of e.g.f. 1/(1 - x*(1 + 3*x + x^2)*exp(x)).
 * https://oeis.org/A308862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A308862
 *  a(0) = 1
 *  a(n) = Σ_{k=1..n} binom(n,k) * k^3 * a(n-k)   for n ≥ 1
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with a(0) = 1 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* OOM – unlikely in test harness */
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* if requested value is already cached, return it immediately */
    if (n < cache_size)
        return cache[n];

    /* grow the cache to accommodate indices up to n */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return 0;                  /* OOM – unlikely in tests */
    cache = tmp;
    /* the new elements will be overwritten as we compute them */

    /* compute any new entries */
    for (idx_t m = cache_size; m <= n; ++m) {
        /* a(0) already set; for m>=1 compute via recurrence */
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;          /* binom(m,0) */

        for (idx_t k = 1; k <= m; ++k) {
            /* update bin to binom(m,k) */
            bin = bin * ((seq_t)m - (seq_t)(k - 1)) / (seq_t)k;

            /* k^3 */
            seq_t kk   = (seq_t)k;
            seq_t k3   = kk * kk * kk;

            /* term = bin * k^3 * a(m-k) */
            seq_t term = bin * k3 * cache[m - k];

            sum += term;
        }
        cache[m] = sum;
    }

    /* expand cache_size to include the newly computed entries */
    cache_size = n + 1;
    return cache[n];
}
