/*
 * A278122  a(n) = 204*2^n - 248.
 * https://oeis.org/A278122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 204*2^n - 248
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is defined for n >= 1; behaviour for n < 1 is irrelevant. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n.  pow2 is defined for arbitrary-precision seq_t values. */
    seq_t val = pow2(n);

    /* Multiply by 204.  Multiplication of seq_t values is exact. */
    val = val * (seq_t)204u;

    /* Subtract 248.  The subtraction is safe because for n >= 1 we have
       204*2^n >= 408 > 248. */
    val -= (seq_t)248u;

    return val;
}
