/*
 * A352306  Expansion of e.g.f. 1/(2 - exp(x) - x^2/2).
 * https://oeis.org/A352306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1; a(n)=binomial(n,2)*a(n-2)+Sum_{k=1..n} binomial(n,k)*a(n-k) */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *a_cache      = NULL;   /* array of terms */
static size_t cache_cap    = 0;      /* allocated capacity */
static idx_t  cache_len    = 0;      /* number of terms stored */

/* Grow the cache to at least new_cap elements */
static void ensure_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;                     /* enough space */
    size_t cap = cache_cap ? cache_cap * 2 : 4;
    while (cap < new_cap)
        cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, cap * sizeof(seq_t));
    a_cache   = tmp;
    cache_cap = cap;
}

seq_t a(idx_t n)
{
    /* initialise cache with a(0) and a(1) on first call */
    if (cache_len == 0) {
        ensure_cache(2);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)1;
        cache_len  = 2;
    }

    if (n < cache_len)
        return a_cache[n];

    /* grow cache up to n */
    ensure_cache((size_t)n + 1);
    for (idx_t m = cache_len; m <= n; ++m) {
        if (m < 2)            /* already set for m = 0,1 */
            continue;

        seq_t sum  = 0;
        seq_t binom = (seq_t)1;   /* binomial(m,0) */

        /* compute sum_{k=1..m} binomial(m,k)*a(m-k) */
        for (idx_t k = 1; k <= m; ++k) {
            /* binomial(m,k) = binomial(m,k-1) * (m-k+1) / k */
            binom = binom * ((seq_t)m - (seq_t)(k - 1)) / (seq_t)k;
            sum += binom * a_cache[m - k];
        }

        /* add the extra binomial(m,2)*a(m-2) (double‑counted by recurrence) */
        seq_t bin2 = ((seq_t)m * ((seq_t)m - 1)) / 2;
        sum += bin2 * a_cache[m - 2];

        a_cache[m] = sum;
    }

    cache_len = n + 1;
    return a_cache[n];
}
