/*
 * A241030  Sum of n-th powers of divisors of 26.
 * https://oeis.org/A241030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 2^n)*(1 + 13^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t pow2n  = pow_int((seq_t)2, n);
    seq_t pow13n = pow_int((seq_t)13, n);
    seq_t one    = (seq_t)1;
    seq_t term1  = one + pow2n;
    seq_t term2  = one + pow13n;
    return term1 * term2;
}
