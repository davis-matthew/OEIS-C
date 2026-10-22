/*
 * A354336  a(n) is the integer w such that (L(2*n)^2, -L(2*n-1)^2, -w) is a
 *          primitive solution to the Diophantine equation 2*x^3 + 2*y^3 + z^3
 *          = 125, where L(n) is the n-th Lucas number (A000032).
 * https://oeis.org/A354336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354336
   Recurrence: a(0)=1, a(1)=11, a(2)=61,
   a(n)=8*a(n-1)-8*a(n-2)+a(n-3)  (n≥3)
   (Lucas-based formulas from OEIS can also compute it.) */

#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache   = NULL;   /* array of values */
    static size_t cache_len = 0;    /* number of stored values (cache indices 0..cache_len-1) */
    static size_t cache_cap = 0;    /* currently allocated capacity */

    /* initialise cache with first three values on first call (or when needed) */
    if (cache_len == 0) {
        ensure_capacity(3, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)61;
        cache_len = 3;
    }

    /* handle trivial cases quickly */
    if (n < cache_len)
        return cache[n];

    /* make sure we have enough space */
    ensure_capacity((size_t)(n + 1), &cache_cap, (void **)&cache, sizeof(seq_t));

    /* compute from current length up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = 8 * cache[i - 1] - 8 * cache[i - 2] + cache[i - 3];
    }
    cache_len = (size_t)(n + 1);

    return cache[n];
}
