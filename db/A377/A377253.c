/*
 * A377253  a(n) = cosh( n*arccosh(3^n) ).
 * https://oeis.org/A377253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = cosh( n*arccosh(3^n) ) Equivalent integer sum: a(n) =
 *            Σ_{k=0..⌊n/2⌋} binomial(n,2k) * 3^{n(n-2k)} * (3^{2n}-1)^k The
 *            implementation iteratively updates the large powers to avoid
 *            repeated exponentiation
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* 9^n – needed for 3^{2n} - 1 */
    seq_t pow9n = big_pow((seq_t)9, (seq_t)n);

    /* r = 3^{2n} - 1 = 9^n - 1 */
    seq_t r = pow9n - (seq_t)1;

    /* 3^{2n} – used to step down the power of 3 in the loop */
    seq_t pow3step = big_pow((seq_t)3, (seq_t)(2 * n));

    /* 3^{n^2} – the first term of the sum (k = 0) */
    seq_t pow3_part = big_pow((seq_t)3, (seq_t)(n * n));

    seq_t r_pow = (seq_t)1;        /* (3^{2n}-1)^k   */
    seq_t sum   = (seq_t)0;

    for (idx_t k = 0; k <= n / 2; ++k)
    {
        /* binomial coefficient binom(n,2k) */
        seq_t bin = binomial(n, 2 * k);

        /* term = bin * 3^{n(n-2k)} * (3^{2n}-1)^k */
        seq_t term = bin * pow3_part * r_pow;
        sum += term;

        /* prepare for the next k */
        if (k < n / 2)
        {
            r_pow   *= r;           /* multiply by (3^{2n}-1) */
            pow3_part /= pow3step;  /* divide by 3^{2n}  */
        }
    }

    return sum;
}
