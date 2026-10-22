/*
 * A354943  a(n) = Sum_{k=0..n} binomial(n,k)^3 * k! * n^(n-k).
 * https://oeis.org/A354943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^3 * k! * n^(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 0, where 0^0 is defined as 1. */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t binom = 1;                    /* C(n,0) */
    seq_t fac   = 1;                    /* 0!   */
    seq_t power = pow_uint((uint64_t)n, n);   /* n^n   */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = binom^3 * fac * power */
        seq_t term = binom;
        term = term * binom * binom;    /* binom^3   */
        term = term * fac;
        term = term * power;
        sum = sum + term;

        if (k == n)
            break;

        /* Update binom to C(n,k+1)   */
        binom = binom * ((seq_t)(n - k)) / ((seq_t)(k + 1));

        /* Update fac to (k+1)!      */
        fac = fac * ((seq_t)(k + 1));

        /* Update power to n^(n-k-1) */
        power = power / ((seq_t)n);
    }

    return sum;
}
