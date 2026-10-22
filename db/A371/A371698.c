/*
 * A371698  Number of partial order-preserving or -reversing transformations
 *          of a chain of length n.
 * https://oeis.org/A371698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A371698: Number of partial order‑preserving or -reversing transformations
   of a chain of length n
   a(n) = 4*Sum_{k=0..n-1} binomial(n-1, k)*binomial(n+k, k) - (1 + n*(2^n - 1)) */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. Return 0 for n == 0. */
    if (n == 0) return (seq_t)0;

    seq_t sum  = 0;
    seq_t b1   = 1; /* binomial(n-1,0) */
    seq_t b2   = 1; /* binomial(n+0,0) */

    for (idx_t k = 0; k < n; ++k)
    {
        /* Add current term to the sum */
        sum += b1 * b2;

        /* Prepare binomials for the next k.  Don't overrun the loop. */
        if (k + 1 < n)
        {
            /* C(n-1, k+1) = C(n-1, k) * (n-1-k) / (k+1) */
            b1 = (b1 * (seq_t)(n - 1 - k)) / (seq_t)(k + 1);

            /* C(n+k+1, k+1) = C(n+k, k) * (n+k+1) / (k+1) */
            b2 = (b2 * (seq_t)(n + k + 1)) / (seq_t)(k + 1);
        }
    }

    /* Multiply the accumulated sum by 4. */
    seq_t term = sum * (seq_t)4;

    /* Subtraction part: 1 + n*(2^n - 1) */
    seq_t pow2_val = pow2(n);                 /* compute 2^n */
    seq_t sub      = ((seq_t)n) * (pow2_val - (seq_t)1) + (seq_t)1;

    return term - sub;
}
