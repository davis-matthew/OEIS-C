/*
 * A360293  a(n) = Sum_{k=0..floor(n/2)} (-1)^k * binomial(n-1-k,k) *
 *          binomial(2*n-4*k,n-2*k).
 * https://oeis.org/A360293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A360293
   Formula: a(n) = Sum_{k=0..⌊n/2⌋} (-1)^k * binomial(n-1-k,k) * binomial(2*n-4*k,n-2*k)
   Recurrence: n*a(n) = 2*(2*n-1)*a(n-1) - 2*(n-2)*a(n-2) + 2*(2*n-7)*a(n-3) - (n-4)*a(n-4)
   Initial values: a(0)=1, a(1)=2, a(2)=6, a(3)=18
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)18;

    /* Store the last four computed values:
       a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)1;   /* a(0)   */
    seq_t a1 = (seq_t)2;   /* a(1)   */
    seq_t a2 = (seq_t)6;   /* a(2)   */
    seq_t a3 = (seq_t)18;  /* a(3)   */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t i_seq = (seq_t)i;

        /* Compute the terms of the recurrence */
        seq_t term1 = (seq_t)2 * ((seq_t)(2 * i - 1)) * a3;      /* 2*(2i-1)*a(n-1) */
        seq_t term2 = (seq_t)2 * ((seq_t)(i - 2)) * a2;          /* 2*(i-2)*a(n-2) */
        seq_t term3 = (seq_t)2 * ((seq_t)(2 * i - 7)) * a1;      /* 2*(2i-7)*a(n-3) */
        seq_t term4 = ((seq_t)(i - 4)) * a0;                     /* (i-4)*a(n-4)   */

        /* Sum positive and negative parts separately to avoid
           underflow on unsigned integers */
        seq_t sum_pos = term1 + term3;
        seq_t sum_neg = term2 + term4;

        seq_t rhs = sum_pos - sum_neg;   /* n*a(n) */
        seq_t ai   = rhs / i_seq;        /* exact division */

        /* Slide window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;   /* a(n) */
}
