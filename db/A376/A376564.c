/*
 * A376564  E.g.f. satisfies A(x) = exp( x*A(x) * (1 + x^2*A(x)^2) ).
 * https://oeis.org/A376564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/3⌋} (n+1)^(n-2k-1) * binomial(n-2k,k) /
 *            (n-2k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ----------------------------------------------------------------------
 * Auxiliary routines
 * ---------------------------------------------------------------------- */

/* Compute base^exp in integer arithmetic (exponentiation by squaring). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1U)
            result = result * base;
        exp >>= 1U;
        if (exp)
            base = base * base;
    }
    return result;
}

/* Compute binomial coefficient C(n,k) using the multiplicative formula. */
static seq_t binomial_idx(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i);   /* numerator */
        res = res / (seq_t)i;             /* exact division */
    }
    return res;
}

/* ----------------------------------------------------------------------
 * Main routine
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t result = (seq_t)0;

    /* Current ratio = n! / m!  (initially m = n, so ratio = 1) */
    seq_t ratio = (seq_t)1;
    /* Current m = n-2k  (starts at n, decreases by 2 each iteration) */
    idx_t m = n;
    seq_t base = (seq_t)(n + (idx_t)1);            /* (n+1) */

    idx_t max_k = n / 3;                           /* floor(n/3) */

    for (idx_t k = 0; k <= max_k; ++k)
    {
        /* Binomial term: binomial(m,k) */
        seq_t bin = binomial_idx(m, k);

        /* Power term: (n+1)^(m-1) */
        seq_t pow_term = pow_seq(base, m - (idx_t)1);

        /* Term = ratio * bin * pow_term */
        seq_t term = ratio * bin;
        term = term * pow_term;

        result = result + term;

        /* Update for next k: ratio *= m * (m-1),
           and decrement m by 2.  Do this only if a further
           iteration will occur. */
        if (k < max_k)
        {
            seq_t mult = (seq_t)m * (seq_t)(m - (idx_t)1);
            ratio = ratio * mult;
            m = m - (idx_t)2;
        }
    }

    return result;
}
