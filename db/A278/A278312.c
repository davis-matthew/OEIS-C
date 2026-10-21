/*
 * A278312  a(n) = denominator of n/(2^(2*n+1)).
 * https://oeis.org/A278312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, changed
 * Formula:   a(n) = denominator of n/(2^(2*n+1)) = 2^(2*n+1 - v2(n)) (v2 is
 *            exponent of 2 in n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special case: a(0) = 1 (the reduced form of 0/2) */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* count trailing zeros in n (exponent of 2 dividing n) */
    idx_t v2 = __builtin_ctzll((unsigned long long)n);

    /* compute exponent for the denominator: 2*n + 1 - v2 */
    idx_t exp = 2 * n + 1 - v2;

    /* return 2^exp using the utility pow2 (handles arbitrary-precision) */
    return pow2(exp);
}
