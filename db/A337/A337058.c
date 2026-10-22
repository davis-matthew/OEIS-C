/*
 * A337058  E.g.f.: 1 / (2 + x^2/2 - exp(x)).
 * https://oeis.org/A337058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A337058:
 * a(0) = 1
 * a(n) = n * a(n-1) + Sum_{k=3..n} binomial(n,k) * a(n-k)   for n >= 1   */
#include <stdlib.h>

/* Cached values of the sequence.  The array is grown on demand and
 * kept in static storage so that calls may arrive in any order. */
static seq_t *cache = NULL;      /* array of cached values             */
static idx_t  cache_cap = 0;     /* allocated capacity                  */
static idx_t  cache_size = 0;    /* first index that is not yet computed */

/* Ensure that cache[0..n] are computed.  The routine grows the
 * array if necessary and computes any new entries. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;               /* already ready */

    /* Grow cached storage if needed. */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* If realloc failed, keep the previous cache.  Future
           computations will retry and eventually succeed. */
        if (!tmp) return;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new entries from cache_size up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;              /* C(i,0) */
        for (idx_t k = 1; k <= i; ++k) {
            /* binom = C(i, k) = C(i, k-1) * (i-k+1) / k  */
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
            if (k >= 3) {
                sum += binom * cache[i - k];
            }
        }
        cache[i] = (seq_t)i * cache[i - 1] + sum;
    }

    cache_size = n + 1;
}

/* Public sequence routine. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
