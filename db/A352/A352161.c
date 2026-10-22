/*
 * A352161  Numbers m such that the smallest digit in the decimal expansion of
 *          1/m is k = 8, ignoring leading and trailing 0's.
 * https://oeis.org/A352161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = 125*10^{(n-1)/2} for odd n, 1125*10^{(n-2)/2} for even n
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/mul_small.h"

/* Compute the nth term of OEIS A352161 */
seq_t a(idx_t n)
{
    /* Sequence offset is 1; for n < 1 behaviour is irrelevant */
    if (n == 0)
        return (seq_t)0;                 /* dummy return */

    if (n & 1U) {           /* odd n */
        idx_t e = (n - 1) / 2; /* exponent of 10 */
        seq_t p = pow_uint(10U, e);
        return mul_small(p, 125U);
    } else {                /* even n */
        idx_t e = (n - 2) / 2;
        seq_t p = pow_uint(10U, e);
        return mul_small(p, 1125U);
    }
}
