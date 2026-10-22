/*
 * A345179  a(1) = 1; a(n) = Sum_{d|n, d < n} binomial(n/d,d) * a(d).
 * https://oeis.org/A345179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A345179
   Formula:  a(1) = 1;
   a(n) = Σ_{d|n, d < n} binomial(n/d, d) * a(d)
   (sum over all proper divisors d of n)
*/

/* include the binomial helper from the utilities */
#include <stdlib.h>
#include "../utils/binomial.h"

/* static cache for memoisation */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static idx_t  max_cached = 0;

/* enlarge the cache if necessary so it can store indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();            /* out of memory */
        /* initialise new entries to zero */
        for (idx_t i = cache_cap; i < (idx_t)new_cap; ++i) new_cache[i] = 0;
        cache      = new_cache;
        cache_cap  = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* outside the OEIS offset */

    ensure_cache(n);

    /* compute all missing values up to n */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        if (k == 1) {                /* base case */
            cache[k] = 1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d != 0) continue;

            /* divisor d */
            if (d < k)
                sum += binomial(k / d, d) * cache[d];

            /* complementary divisor k/d */
            idx_t d2 = k / d;
            if (d2 != d && d2 < k)
                sum += binomial(k / d2, d2) * cache[d2];
        }
        cache[k] = sum;
    }
    if (n > max_cached) max_cached = n;
    return cache[n];
}
