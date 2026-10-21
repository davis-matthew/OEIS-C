/*
 * A288312  Number of endofunctions on [2n] such that the image size equals n.
 * https://oeis.org/A288312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Stirling2(2n,n) * n! * binomial(2n,n) Equivalent to a(n)
 *            = ( Σ_{j=0}^n (-1)^j * binomial(n,j) * (n-j)^(2n) ) *
 *            binomial(2n,n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the inclusion–exclusion sum:
       S = Σ_{j=0}^n (-1)^j * binomial(n,j) * (n-j)^(2n)   */
    seq_t sum_pos = 0;   /* terms with even j  */
    seq_t sum_neg = 0;   /* terms with odd  j  */
    seq_t two_n   = (seq_t)(2 * n);

    for (idx_t j = 0; j <= n; ++j)
    {
        seq_t comb  = binomial(n, j);                 /* C(n,j)          */
        seq_t pow_j = big_pow((seq_t)(n - j), two_n); /* (n-j)^(2n)      */
        seq_t term  = comb * pow_j;                   /* C(n,j)*(n-j)^(2n) */

        if ((j & 1) == 0)  /* even j  */
            sum_pos += term;
        else                /* odd  j  */
            sum_neg += term;
    }

    /* The sum S is positive, so sum_pos >= sum_neg. */
    seq_t S = sum_pos - sum_neg;

    /* a(n) = S * binomial(2n,n) */
    seq_t bin = binomial(n * 2, n);
    return S * bin;
}
