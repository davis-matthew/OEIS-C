/*
 * A388412  a(n) = Sum_{k=0..n} 4^(n-k) * binomial(n,k) * binomial(n+2,k).
 * https://oeis.org/A388412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 4^(n-k) * binomial(n,k) * binomial(n+2,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A388412.  Uses binomial and pow_int utilities. */

#include "../utils/binomial.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 immediately */
    if (n == 0)
        return (seq_t)1;

    seq_t result = 0;
    /* 4^n as a starting power */
    seq_t pow4 = pow_int((seq_t)4, n);

    /* Start with binomial coefficients C(n,0) and C(n+2,0) */
    seq_t Cnk = (seq_t)1;          /* C(n,0) */
    seq_t Cn2k = (seq_t)1;         /* C(n+2,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = 4^(n-k) * C(n,k) * C(n+2,k) */
        seq_t temp = Cnk * Cn2k;
        seq_t term = pow4 * temp;
        result += term;

        /* prepare for next iteration */
        pow4 /= (seq_t)4;             /* 4^(n-(k+1)) */

        if (k < n)
        {
            /* Update binomial coefficients for k+1 using the recurrence
               C(N, k+1) = C(N, k) * (N - k) / (k + 1).  The divisions are exact. */
            Cnk = Cnk * (n - k) / (k + 1);
            Cn2k = Cn2k * (n + 2 - k) / (k + 1);
        }
    }

    return result;
}
