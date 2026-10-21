/*
 * A250271  Number of length n+1 0..2 arrays with the sum of the squares of
 *          adjacent differences multiplied by some arrangement of +-1 equal
 *          to zero.
 * https://oeis.org/A250271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3 a(n) = 5*3^(n-1) - (2^n - 2)*n for n > 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A250271 using the closed‑form
   expression. It works for any integer type provided for seq_t and
   idx_t, including arbitrary‑precision classes. */
#include <stdint.h>

/* Multiply a seq_t by a small integer base repeatedly to compute base^exp.
   Works efficiently for small bases (e.g., 2,3) as used in this sequence. */
static seq_t pow_small(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    if (n <= (idx_t)1)
        return (seq_t)3;

    /* 3^(n-1) */
    seq_t pow3 = pow_small((seq_t)3, n - (idx_t)1);
    /* 2^n */
    seq_t pow2 = pow_small((seq_t)2, n);

    seq_t term1 = (seq_t)5 * pow3;
    seq_t term2 = (pow2 - (seq_t)2) * (seq_t)n;

    return term1 - term2;
}
