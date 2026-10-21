/*
 * A276289  Expansion of x*(1 + x)/(1 - 2*x)^3.
 * https://oeis.org/A276289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 12*a(n-2) + 8*a(n-3); a(0)=0, a(1)=1, a(2)=7
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   ensure_capacity, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of computed terms */
    static size_t cache_cap = 0;     /* currently allocated capacity */

    /* Ensure we have storage for indices 0..n */
    ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Compute missing terms up to and including n */
    while (cache_len <= n) {
        idx_t i = cache_len;
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)7;
        } else {
            /* recurrence: a(i) = 6*a(i-1) - 12*a(i-2) + 8*a(i-3) */
            seq_t part1 = mul_small(cache[i - 1], 6);
            seq_t part2 = mul_small(cache[i - 2], 12);
            seq_t part3 = mul_small(cache[i - 3], 8);
            seq_t sum   = part1 + part3;   /* part1 + part3 >= part2 for i>=3 */
            seq_t res   = sum - part2;
            cache[i] = res;
        }
        cache_len++;
    }
    return cache[n];
}
