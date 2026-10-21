/*
 * A284374  a(1) = a(2) = 1; a(n) is the largest prime <= (a(n-a(n-1)) +
 *          a(n-a(n-2))) for n > 2.
 * https://oeis.org/A284374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 1; a(n) = largest prime ≤ (a(n-a(n-1)) +
 *            a(n-a(n-2))) for n > 2
 * Range:     u8 n <= 366, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n == 1 || n == 2) return (seq_t)1;

    /* Make sure we have room for indices up to n (including 0 for safety). */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise base cases if not already done. */
    if (cache[1] == 0) cache[1] = (seq_t)1;
    if (cache[2] == 0) cache[2] = (seq_t)1;

    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] != 0) continue;             /* already computed */

        /* The indices used by the recurrence.  They are guaranteed to be positive. */
        idx_t idx1 = i - (idx_t)cache[i-1];
        idx_t idx2 = i - (idx_t)cache[i-2];

        seq_t sum = cache[idx1] + cache[idx2];
        seq_t val = sum;

        /* Find largest prime ≤ sum.  The gaps between consecutive primes are
           small compared with sum, so a linear search downward is fast enough. */
        while (!is_prime_big(val)) {
            val--;          /* seq_t supports post‑decrement */
        }

        cache[i] = val;
    }

    return cache[n];
}
