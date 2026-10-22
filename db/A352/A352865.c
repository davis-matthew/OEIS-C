/*
 * A352865  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/2)} (-1)^k *
 *          binomial(n-k,k) * a(n-2*k-1).
 * https://oeis.org/A352865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} (-1)^k * binomial(n-k, k) *
 *            a(n-2*k-1); a(0) = 1
 * Range:     i8 n <= 9, i16 n <= 14, i32 n <= 23, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"
#include "../utils/binomial.h"
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = (idx_t)-1;

seq_t a(idx_t n)
{
    /* Make sure we have space for indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialize first term if this is the first call. */
    if (last_computed == (idx_t)-1) {
        cache[0] = (seq_t)1;
        last_computed = 0;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        idx_t maxk = (i > 0) ? (i - 1) / 2 : 0;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t bin = binomial(i - k, k);
            seq_t term = bin * cache[i - 2 * k - 1];
            if (k & 1)
                sum -= term;
            else
                sum += term;
        }
        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
