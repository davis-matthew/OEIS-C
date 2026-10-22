/*
 * A367376  Expansion of the e.g.f. (exp(x) / (5 - 4*exp(x)))^(4/5).
 * https://oeis.org/A367376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, for n>=1 a(n) = 4*a(n-1) + 4*sum_{k=1}^{n-1}
 *            binom(n-1,k)*a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* cached values of a(n) */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t cache_len = 0;      /* number of values currently stored */

/* ensure storage for indices up to n                                 */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t newcap = (cache_cap == 0) ? (n + 1) : (cache_cap * 2);
        if (newcap <= (size_t)n + 1) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */
        cache = tmp;
        cache_cap = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;               /* base value */

    ensure_cache(n);

    /* initialize cache[0] on first use */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* compute values up to n that are not yet stored */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t sum  = 0;
        seq_t binom = 1;                    /* binom(n-1,0) */

        for (idx_t i = 1; i <= k - 1; ++i) {
            sum += binom * cache[i];
            if (i != k - 1)                /* avoid division by 0   */
                binom = binom * (seq_t)(k - i) / (seq_t)i;
        }
        cache[k] = 4 * cache[k - 1] + 4 * sum;
    }
    cache_len = n + 1;
    return cache[n];
}
