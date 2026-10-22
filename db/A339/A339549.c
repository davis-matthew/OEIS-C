/*
 * A339549  a(n) is the product of the binary weights (A000120) of the
 *          divisors of n.
 * https://oeis.org/A339549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∏_{d|n} A000120(d) (product of binary popcounts of all
 *            divisors of n)
 * Range:     u8 n <= 29, u16 n <= 119, u32 n <= 419, u64 n <= 2519,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_isqrt_u64.h"

/* Popcount of a 64‑bit integer – returns a value that fits in seq_t (≤ 64). */
static inline seq_t popcount_uint64(uint64_t x)
{
    seq_t c = 0;
    while (x) {
        c += (seq_t)(x & 1ULL);
        x >>= 1;
    }
    return c;
}

/* Sequence generator with external linkage. */
seq_t a(idx_t n)
{
    /* n ≥ 1 by the OEIS offset; behaviour for n == 0 is undefined. */
    seq_t prod = (seq_t)1;

    idx_t sq = floor_isqrt_u64(n);          /* integer sqrt of n */

    for (idx_t d = 1; d <= sq; ++d) {
        if (n % d == 0) {
            uint64_t d1 = (uint64_t)d;
            prod *= popcount_uint64(d1);

            uint64_t d2 = n / d;
            if (d2 != d1) {                /* avoid double‑counting a perfect square root */
                prod *= popcount_uint64(d2);
            }
        }
    }
    return prod;
}
