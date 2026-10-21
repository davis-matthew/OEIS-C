/*
 * A241478  a(n) = 4^n*(n/4 + binomial(n-1/2, -1/2)).
 * https://oeis.org/A241478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4ⁿ * ( n/4 + binomial(n-1/2, -1/2) ) * = binomial(2n, n)
 *            + n * 4^(n-1) for n ≥ 1, a(0) = 1 * This follows from the
 *            generating function G(x) = x/(1-4x)² + 1/√(1-4x)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   binom, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* binomial(2n, n) */
    seq_t bin = binom(2 * n, n);

    /* 4^(n-1) */
    seq_t pow4_val = pow4(n - 1);

    /* n * 4^(n-1) */
    seq_t mul = (seq_t)n * pow4_val;

    /* a(n) = binomial(2n, n) + n*4^(n-1) */
    return bin + mul;
}
