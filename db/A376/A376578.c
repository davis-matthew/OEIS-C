/*
 * A376578  E.g.f. A(x) satisfies A(x) = exp(x*(1 + x^3)*A(x)).
 * https://oeis.org/A376578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/4⌋} (n-3k+1)^(n-3k-1) * binomial(n-3k, k)
 *            / (n-3k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper functions (from utils): */
#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/binomial.h"    /* seq_t binomial(idx_t a, idx_t b); */

/* Sequence value at index n. */
seq_t a(idx_t n)
{
    /* Handle n == 0 separately to avoid underflow in m-1 and division by zero. */
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t m   = n;          /* m = n - 3*k, starts at n */
    seq_t prod = 1;         /* prod = n! / m!   (product of the next 3 integers each step) */

    for (idx_t k = 0; ; ++k)
    {
        /* Compute (m+1)^(m-1).  For m <= 1 the power is 1. */
        seq_t pow_val;
        if (m <= 1)
            pow_val = (seq_t)1;
        else
            pow_val = big_pow((seq_t)(m + 1), (seq_t)(m - 1));

        /* Binomial coefficient binomial(m, k). */
        seq_t bin = binomial(m, k);

        /* term = prod * pow_val * bin */
        seq_t term = prod;
        term = term * pow_val;
        term = term * bin;

        sum = sum + term;

        /* Prepare for the next k (m decreases by 3).  If m < 3 we are done. */
        if (m < 3)
            break;
        /* prod <- prod * m * (m-1) * (m-2)  (to get n! / (m-3)! for next iteration) */
        prod = prod * (seq_t)m;
        prod = prod * (seq_t)(m - 1);
        prod = prod * (seq_t)(m - 2);

        m -= 3;
    }

    return sum;
}
