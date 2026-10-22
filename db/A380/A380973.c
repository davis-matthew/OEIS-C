/*
 * A380973  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x*exp(3*x))
 *          ).
 * https://oeis.org/A380973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (3*k)^(n-k) * (n+1)^(k-1) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (In practice the sum is taken over k = 1..n; the term k = 0 contributes
   only when n = 0, yielding a(0) = 1.) */

#include <stdint.h>

/* Helper: integer power by repeated squaring. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)              /* a(0) = 1 */
        return 1;

    seq_t sum = 0;
    seq_t bin = 1;           /* binomial(n, 0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update binomial coefficient to binomial(n, k) */
        bin = bin * (n - k + 1) / k;

        /* Compute (3*k)^(n-k) and (n+1)^(k-1) */
        seq_t pow1 = pow_seq((seq_t)(3 * k), n - k);
        seq_t pow2 = pow_seq((seq_t)(n + 1), k - 1);

        /* term = bin * pow1 * pow2 */
        seq_t term = bin * pow1;
        term *= pow2;

        sum += term;
    }

    return sum;
}
