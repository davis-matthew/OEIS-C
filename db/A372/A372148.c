/*
 * A372148  a(n) = A371764(n, 2).
 * https://oeis.org/A372148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(4*3^n - 9*2^n + 7) - [n = 1]
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* ensures seq_pow is declared */

seq_t a(idx_t n)
{
    /* Compute 3^n and 2^n using the provided big integer power routine. */
    seq_t pow3 = seq_pow((seq_t)3, n);
    seq_t pow2 = seq_pow((seq_t)2, n);

    /* Apply the formula.  All intermediate results are in seq_t. */
    seq_t term = (seq_t)4 * pow3 - (seq_t)9 * pow2 + (seq_t)7;
    seq_t res  = (seq_t)2 * term;

    /* Subtract 1 if n == 1 because the sequence is offset at 1. */
    if (n == (idx_t)1)
        res--;

    return res;
}
