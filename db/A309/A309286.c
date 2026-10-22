/*
 * A309286  a(0) = 0, a(1) = 1, and for any n > 1, a(n) = Sum_{k > 1} (-1)^k *
 *          a(floor(n/k^2)).
 * https://oeis.org/A309286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 0, a(1) = 1, a(n) = Σ_{k>1} (-1)^k * a( floor(n/k²) ) for
 *            n > 1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache      = NULL;   /* memoisation array */
static idx_t  cache_cap  = 0;      /* allocated size (indices 0..cap-1) */
static idx_t  cache_len  = 0;      /* number of initialised indices
                                     (cache_len is the first index not yet set) */

/* Expand the cache so that indices 0..n are addressable. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;               /* already large enough */

    size_t new_cap = (size_t)n + 1;
    if (new_cap < 4)           /* minimal capacity */
        new_cap = 4;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* out of memory */

    cache     = tmp;
    cache_cap = new_cap;
}

/* Compute a(i) for all i in [cache_len .. n] (if needed). */
static void compute_up_to(idx_t n)
{
    ensure_cache(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            /* For each k≥2 with k² ≤ i: */
            for (idx_t k = 2; k <= i / k; ++k) {
                idx_t sub = i / (k * k);      /* floor(i / k²) */
                seq_t term = cache[sub];
                if (k & 1)                       /* (−1)^k for odd k */
                    sum -= term;
                else
                    sum += term;
            }
            cache[i] = sum;
        }
    }
    cache_len = n + 1;   /* next uninitialised index */
}

seq_t a(idx_t n)
{
    if (n >= cache_len)
        compute_up_to(n);
    return cache[n];
}
