/*
 * A273611  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 809", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (2150*4^(n-4) - 4*3^(n-4) - 12*2^n - 8)/3 (for n>4), with
 *            a(0)=1, a(1)=4, a(2)=28, a(3)=144, a(4)=652
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)28;
    if (n == 3) return (seq_t)144;
    if (n == 4) return (seq_t)652;

    /* n > 4 */
    idx_t exp4 = 2 * ((idx_t)n - 4);           /* exponent for 4^(n-4) = 2^(2*(n-4)) */
    seq_t pow4 = pow2(exp4);                  /* 4^(n-4) */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n - 4); /* 3^(n-4) */
    seq_t pow2n = pow2(n);                    /* 2^n */

    seq_t numerator = ((seq_t)2150 * pow4
                       - (seq_t)4 * pow3
                       - (seq_t)12 * pow2n
                       - (seq_t)8);
    seq_t result = numerator / (seq_t)3;
    return result;
}
