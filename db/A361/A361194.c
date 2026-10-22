/*
 * A361194  E.g.f. satisfies A(x) = exp( -3*x*A(x) ) / (1-x).
 * https://oeis.org/A361194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..n} (-3)^k * (k+1)^(k-1) * binomial(n,k) /
 *            k!. The implementation uses the multiplicative form of binomial
 *            and factorial. All operations are performed with seq_t (possibly
 *            arbitrary precision)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>    /* for uint64_t, may be omitted if already defined */

/* fast exponentiation for seq_t (works for negative bases) */
static seq_t pow_seq(seq_t base, int exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0)
    {
        if (exp & 1) result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* factorial of n */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n = fact_n * (seq_t)i;

    seq_t sum = 0;
    seq_t binom = 1;      /* binomial(n,0) */
    seq_t fact_k = 1;     /* k! */
    seq_t pow_neg = 1;    /* (-3)^k */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = binom * fact_n / fact_k * (-3)^k * (k+1)^(k-1) */
        seq_t term = binom * fact_n;
        term = term / fact_k;           /* exact division */

        term = term * pow_neg;          /* multiply by (-3)^k */

        seq_t pow_kplus1;
        if (k == 0)
            pow_kplus1 = 1;              /* (1)^( -1) is treated as 1 */
        else
            pow_kplus1 = pow_seq((seq_t)(k + 1), (int)(k - 1));

        term = term * pow_kplus1;

        sum += term;

        /* update values for next k */
        if (k < n)
        {
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);  /* binom(n,k+1) */
            fact_k = fact_k * (seq_t)(k + 1);                 /* (k+1)!   */
        }

        pow_neg = pow_neg * (seq_t)(-3);          /* (-3)^(k+1) for next round */
    }

    return sum;
}
