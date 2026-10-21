/*
 * A255807  Expansion of e.g.f.: exp(Sum_{k>=1} k^2 * x^k).
 * https://oeis.org/A255807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n-3)*a(n-1) - 2*(n-1)*(3*n-8)*a(n-2) +
 *            (n-1)*(n-2)*(4*n-11)*a(n-3) - (n-1)*(n-2)*(n-3)*(n-4)*a(n-4);
 *            Initial values: a(0) = 1, a(1) = 1, a(2) = 9, a(3) = 79
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)9;
    if (n == 3)
        return (seq_t)79;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)9;   /* a(2) */
    seq_t a3 = (seq_t)79;  /* a(3) */
    seq_t ai, term1, term2, term3, term4, sum_pos, sum_neg;

    for (idx_t k = 4; k <= n; ++k)
    {
        /* term1 = (4*k-3) * a(n-1) */
        term1 = (seq_t)((uint64_t)4 * k - 3);
        term1 *= a3;

        /* term2 = 2*(k-1)*(3*k-8) * a(n-2) */
        term2 = (seq_t)2;
        term2 *= (seq_t)(k - 1);
        term2 *= (seq_t)(3 * k - 8);
        term2 *= a2;

        /* term3 = (k-1)*(k-2)*(4*k-11) * a(n-3) */
        term3 = (seq_t)(k - 1);
        term3 *= (seq_t)(k - 2);
        term3 *= (seq_t)(4 * k - 11);
        term3 *= a1;

        /* term4 = (k-1)*(k-2)*(k-3)*(k-4) * a(n-4) */
        term4 = (seq_t)(k - 1);
        term4 *= (seq_t)(k - 2);
        term4 *= (seq_t)(k - 3);
        term4 *= (seq_t)(k - 4);
        term4 *= a0;

        sum_pos = term1 + term3;
        sum_neg = term2 + term4;
        ai = sum_pos - sum_neg;

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
