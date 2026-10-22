/*
 * A397515  a(n) = Sum_{k=0..n} binomial(4*n+2*k-3,k-1).
 * https://oeis.org/A397515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n+2*k-3, k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes utilities for division and gcd */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) is the first term */
    if (n == 0)
        return 0;

    /* term_k = C(4*n+2k-3, k-1)
       Start with k = 1: term = C(4*n-1,0) = 1 */
    seq_t sum  = 1;
    seq_t term = 1;

    for (idx_t k = 1; k < n; ++k) {
        /* Compute numerator and denominator for the ratio */
        seq_t a_val = (seq_t)4 * n + (seq_t)2 * k - 1;
        seq_t b_val = (seq_t)4 * n + (seq_t)2 * k - 2;
        seq_t num   = a_val * b_val;                           /* (4n+2k-1)(4n+2k-2) */
        seq_t denom = (seq_t)k * ((seq_t)4 * n + (seq_t)k - 1);  /* k*(4n+k-1) */

        /* Reduce fraction to avoid overflow */
        seq_t g = gcd(num, denom);
        num   /= g;
        denom /= g;

        /* Update term: term = term * num / denom  (exact division) */
        term = term / denom;
        term = term * num;

        /* Accumulate */
        sum += term;
    }

    return sum;
}
