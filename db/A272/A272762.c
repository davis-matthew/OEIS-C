/*
 * A272762  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 822", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 5 - 2^(1+n) + 2^(1+2*n) - 4*3^n + 2^(2+n)*n (conjectured
 *            closed form for OEIS A272762) a(0) = 1, a(1) = 5, a(2) = 25,
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"   /* provides seq_t pow_seq(seq_t base, int exp) */

seq_t a(idx_t n)
{
    /* 2^n, 2^(2n), 3^n */
    seq_t pow2_n   = pow_seq((seq_t)2, (int)n);
    seq_t pow2_2n  = pow_seq((seq_t)2, (int)(2 * n));
    seq_t pow3_n   = pow_seq((seq_t)3, (int)n);

    /* compute the expression safely with unsigned arithmetic */
    seq_t result = (seq_t)5;                          /* +5 */
    result -= (seq_t)2 * pow2_n;                      /* -2 * 2^n */
    result += (seq_t)2 * pow2_2n;                     /* +2 * 2^(2n) */
    result -= (seq_t)4 * pow3_n;                      /* -4 * 3^n */
    result += (seq_t)4 * pow2_n * (seq_t)n;           /* +4 * 2^n * n */

    return result;
}
