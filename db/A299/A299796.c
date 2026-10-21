/*
 * A299796  Largest number k that is not the sum of 3 squares and a
 *          nonnegative (2n+5)-th power.
 * https://oeis.org/A299796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{2<=i<=n+1, j>=0} {4^i * (8j+7) : 4^i * (8j+7) <
 *            3^(2n+5)}
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Defensive guard – the sequence is defined for n >= 1. */
    if (n < 1) {
        return (seq_t)0;
    }

    /* Compute the threshold T = 3^(2n+5). */
    idx_t exp = 2*n + 5;
    seq_t T = pow_int((seq_t)3, exp);

    seq_t max_val = 0;

    /* Start with i = 2: pow4 = 4^2 = 16.  M = floor((T-1)/pow4). */
    seq_t pow4 = (seq_t)16;
    seq_t M = (T - 1) / pow4;

    /* Loop over i = 2 … n+1. */
    for (idx_t i = 2; i <= n + 1; ++i)
    {
        if (M >= (seq_t)7) {
            /* k = largest 8j+7 <= M. */
            seq_t r = (M - (seq_t)7) % (seq_t)8;   /* remainder modulo 8 */
            seq_t k = M - r;                       /* k ≡ 7 (mod 8) */
            seq_t v = pow4 * k;
            if (v > max_val) {
                max_val = v;
            }
        }
        /* Prepare for the next i. */
        if (i != n + 1) {
            pow4 *= (seq_t)4;  /* 4^(i+1) from 4^i */
            M /= (seq_t)4;     /* floor(.../4) gives M for next i */
        }
    }

    return max_val;
}
