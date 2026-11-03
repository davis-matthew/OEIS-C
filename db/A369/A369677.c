/*
 * A369677  a(n) = Product_{k=0..n} (2^k + 5^(n-k)).
 * https://oeis.org/A369677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (2^k + 5^(n-k))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t pow2 = 1;             /* 2^k  */
    seq_t pow5 = power5(n);     /* 5^(n-k) when k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = pow2 + pow5;   /* 2^k + 5^(n-k) */
        result *= term;

        pow2 *= 2;     /* advance to next 2^k    */
        pow5 /= 5;     /* advance to next 5^(n-k) */
    }
    return result;
}
