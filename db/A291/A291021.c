/*
 * A291021  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - S - S^2 - S^3 + S^4
 *          + S^5.
 * https://oeis.org/A291021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2) + 14*a(n-3) - 9*a(n-4) + 2*a(n-5)
 *            a(0)=1, a(1)=3, a(2)=9, a(3)=25, a(4)=67
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

/* compute the next value using the recurrence */
static inline seq_t compute_next(idx_t i, seq_t *buf)
{
    /* seq_t arithmetic will work for both builtin integer types
       and arbitrary‑precision classes. */
    seq_t res = 6 * buf[i-1];
    res -= (seq_t)13 * buf[i-2];
    res += (seq_t)14 * buf[i-3];
    res -= (seq_t)9 * buf[i-4];
    res += (seq_t)2 * buf[i-5];
    return res;
}

/* public function */
seq_t a(idx_t n)
{
    /* initialise cache with the first five terms */
    if (cache_len == 0) {
        ensure_cache(5, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)67;
        cache_len = 5;
    }

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow cache to accommodate up to n */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence applies for i >= 5, for i < 5 values are already set */
        if (i < 5) continue;
        cache[i] = compute_next(i, cache);
    }
    cache_len = n + 1;

    return cache[n];
}
