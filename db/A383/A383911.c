/*
 * A383911  Expansion of Product_{k=0..4} (1 + k*x)/(1 - k*x).
 * https://oeis.org/A383911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 70*4^n - 140*3^n + 45*2^(n+1) - 20 for n > 0 a(0) = 1
 *            Uses the closed‑form expression which evaluates in O(log n) time
 *            and works for any seq_t (unsigned integer or
 *            arbitrary‑precision)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1U;

    /* Compute the terms of the formula.  All intermediate results fit
       in seq_t because they are sums/differences of the final
       non‑negative values. */
    seq_t term1 = pow4(n);       /* 4^n   */
    term1 = term1 * (seq_t)70U;  /* 70*4^n */

    seq_t term3 = pow2(n + 1);   /* 2^(n+1) */
    term3 = term3 * (seq_t)45U;  /* 45*2^(n+1) */

    seq_t term2 = pow3(n);       /* 3^n   */
    term2 = term2 * (seq_t)140U; /* 140*3^n */

    seq_t result = term1 + term3;
    result -= term2;
    result -= (seq_t)20U;

    return result;
}
