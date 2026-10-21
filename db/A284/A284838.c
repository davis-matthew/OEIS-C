/*
 * A284838  Number of edges in the n-Keller graph.
 * https://oeis.org/A284838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n-1)*(4^n-3^n-n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) is outside the defined offset; return 0 if called */
    if (n == 0) {
        return (seq_t)0;
    }

    /* compute 2^(2*n-1) */
    seq_t exp_two = (seq_t)(2 * n - 1);
    seq_t two_pow = big_pow((seq_t)2, exp_two);

    /* compute 4^n and 3^n */
    seq_t four_pow  = big_pow((seq_t)4, (seq_t)n);
    seq_t three_pow = big_pow((seq_t)3, (seq_t)n);

    /* compute the bracket */
    seq_t inside = four_pow - three_pow - (seq_t)n;

    /* return the product */
    return two_pow * inside;
}
