/*
 * A332692  a(n) = n! * Laguerre(n, 2*n).
 * https://oeis.org/A332692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Laguerre(n, 2*n) (Laguerre defined by L_n(x) =
 *            Σ_{k=0}^n binom(n,k) * (-x)^k / k! )
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t i, k;
    seq_t fact_n = 1;          /* n! */
    seq_t x = (seq_t)((idx_t)2 * n);   /* 2*n as seq_t */

    /* Compute n! */
    for (i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t binom = 1;          /* binom(n,0) */
    seq_t pow_k = 1;         /* (-x)^0 */
    seq_t fact_div = fact_n; /* n! / 0! = n! */
    seq_t sum = 0;

    for (k = 0; k <= n; ++k)
    {
        /* term = binom(n,k) * (n! / k!) * (-x)^k */
        seq_t term = binom * fact_div * pow_k;
        sum += term;

        if (k == n) break; /* no more updates after last term */

        /* Update binom(n,k+1) = binom(n,k) * (n-k) / (k+1) */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);

        /* Update power: (-x)^{k+1} = (-x)^k * (-x) */
        pow_k = pow_k * -x;

        /* Update factorial division: n! / (k+1)! = (n! / k!) / (k+1) */
        fact_div = fact_div / (seq_t)(k + 1);
    }

    return sum;
}
