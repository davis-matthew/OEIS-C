/*
 * A293040  E.g.f.: exp(1 + x + x^2/2! + x^3/3! + x^4/4! - exp(x)).
 * https://oeis.org/A293040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = 0 for 1 ≤ n < 5 a(n) = -Sum_{k=5..n}
 *            binomial(n-1,k-1) * a(n-k) for n ≥ 5
 * Range:     i8 n <= 9, i16 n <= 14, i32 n <= 20, i64 n <= 31, i128 n <= 49,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;      /* array storing computed values            */
static size_t cache_cap = 0;     /* allocated capacity (indices 0..cap-1)    */
static size_t cache_len = 0;     /* number of values already computed       */

seq_t a(idx_t n) {
    /* Return a cached value if already computed */
    if (n < (idx_t)cache_len) return cache[n];

    /* Ensure the cache can hold indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute missing terms one by one */
    idx_t start = (idx_t)cache_len;
    for (idx_t i = start; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i < 5) {
            val = (seq_t)0;
        } else {
            seq_t sum = (seq_t)0;
            /* sum_{k=5..i} binomial(i-1,k-1) * a(i-k) */
            for (idx_t k = 5; k <= i; ++k) {
                seq_t binom = binomial(i-1, k-1);   /* pre‑provided util */
                sum += binom * cache[i - k];
            }
            val = -sum;
        }
        cache[i] = val;
    }

    cache_len = n + 1;   /* all indices up to n are now set */
    return cache[n];
}
