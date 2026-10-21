/*
 * A241031  Sum of n-th powers of divisors of 28.
 * https://oeis.org/A241031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n + 2^n + 1) * (7^n + 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 4^n, 2^n and 7^n with arbitrary‑precision exponentiation */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);

    /* Evaluate the two factors of the formula */
    seq_t part1 = pow4 + pow2 + (seq_t)1;
    seq_t part2 = pow7 + (seq_t)1;

    return part1 * part2;
}
