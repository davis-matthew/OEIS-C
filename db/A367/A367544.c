/*
 * A367544  Euclid's triangle A217831 represented as decimal numbers.
 * https://oeis.org/A367544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=0..n} 2^k * [gcd(k, n) = 1]
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_gcd_idx.h"

/* Euclidean algorithm for idx_t (uint64_t). */

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition. */
    if (n == 0) return 0;

    seq_t sum   = 0;
    seq_t value = 1;          /* 2^0 */

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {
            /* gcd(0, n) = n, only equal to 1 when n == 1. */
            if (n == 1) sum += value;
        } else if (_gcd_idx(k, n) == 1) {
            sum += value;
        }
        /* Prepare value for next iteration: 2^(k+1). */
        if (k < n) value <<= 1;
    }
    return sum;
}
