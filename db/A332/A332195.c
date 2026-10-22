/*
 * A332195  a(n) = 10^(2*n+1) - 4*10^n - 1.
 * https://oeis.org/A332195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(2*n+1) - 4*10^n - 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed using the utility function seq_pow for arbitrary precision arithmetic */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Exponents fit in idx_t for the intended call range */
    idx_t exp1 = 2 * n + 1;
    idx_t exp2 = n;

    /* 10^(2*n+1) */
    seq_t pow1 = seq_pow((seq_t)10, exp1);

    /* 10^n */
    seq_t pow2 = seq_pow((seq_t)10, exp2);

    /* a(n) = pow1 - 4*pow2 - 1 */
    seq_t result = pow1;
    result -= (seq_t)4 * pow2;
    result -= (seq_t)1;

    return result;
}
