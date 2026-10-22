/*
 * A353551  a(n) = Sum_{k=1..n} tau(k^3), where tau is the number of divisors
 *          function A000005.
 * https://oeis.org/A353551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} tau(k^3) where tau(m) is the number of
 *            positive divisors of m. Build a small on‑demand cache and
 *            compute each term by calling the provided divisor_count_u64()
 *            helper. n is a nonnegative uint64_t. The function is safe for
 *            all n that fit in idx_t and fast enough for n up to ~10...
 * Range:     u8 n <= 23, u16 n <= 1187, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL; /* cache[i] = a(i) for 0 <= i <= cached */
    static idx_t  cached_upto = 0;   /* highest index already computed */

    /* Allocate cache for the requested index if it does not yet exist. */
    if (!cache || n >= cached_upto) {
        /* Ensure allocation for indices 0..n */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* out of memory – return 0 as fallback */
        cache = tmp;
        if (!cache[0]) cache[0] = (seq_t)0;   /* initialise a(0) = 0 */

        /* Compute missing terms sequentially. */
        for (idx_t k = cached_upto + 1; k <= n; ++k) {
            uint64_t kk = (uint64_t)k;
            /* k^3 < 10^12 for k <= 10 000; safe in 64‑bit unsigned. */
            uint64_t kk3 = kk * kk * kk;
            seq_t tau = divisor_count_u64(kk3);
            cache[k] = cache[k - 1] + tau;
        }
        cached_upto = n;
    }
    return cache[n];
}
