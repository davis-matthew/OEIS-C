/*
 * A345078  a(0) = 1; a(n) = 7 * Sum_{k=1..n} binomial(n,k) * a(k-1).
 * https://oeis.org/A345078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 7 * Sum_{k=1..n} binomial(n,k) * a(k-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed_max = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Ensure space for the first element and initialise a(0) = 1 */
        ensure_cache(0, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        computed_max = 0;
        initialized = true;
    }

    ensure_cache(n, &cache, &cache_cap);

    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t C   = (seq_t)1;   /* binomial coefficient C(i,0) */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                /* Update C to C(i,k) using the recurrence          */
                C = (C * (seq_t)(i - k + 1)) / (seq_t)k;
                sum += C * cache[k - 1];
            }
            cache[i] = (seq_t)7 * sum;
        }
        computed_max = n;
    }

    return cache[n];
}
