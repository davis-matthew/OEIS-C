/*
 * A337992  a(n) = Sum_{k=0..n} (n+1)*2^(n+k)*hypergeom([-n, k-n+1], [2],
 *          1/2). Row sums of A337617.
 * https://oeis.org/A337992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (if (n = k) then 2^n*(2^(n + 1) - 1) else
 *            2^(2*k + 1)*Sum_{j=0..n-k} (-1)^j*2^(n - k - j)*binomial(n + 1,
 *            j)*binomial(2*n - j - k, n))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function uses the helper functions `pow2` and `binomial`, both provided in the utils library. */
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow2_n    = pow2(n);          /* 2^n      */
    seq_t pow2_n1   = pow2(n + 1);      /* 2^(n+1)  */

    idx_t k;
    for (k = 0; k <= n; ++k)
    {
        seq_t term;

        if (k == n)                         /* term when k == n */
        {
            term = pow2_n * (pow2_n1 - (seq_t)1);
        }
        else                                /* k < n */
        {
            /* inner sum over j */
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            idx_t j;
            idx_t maxj = n - k;

            for (j = 0; j <= maxj; ++j)
            {
                seq_t binom1 = binomial(n + 1, j);
                seq_t binom2 = binomial(2 * n - j - k, n);
                seq_t powj   = pow2(n - k - j);

                seq_t termj = binom1;
                termj *= binom2;
                termj *= powj;

                if ((j & 1) == 0)
                    sum_pos += termj;
                else
                    sum_neg += termj;
            }

            /* the inner sum is non‑negative; in the unlikely event that the odd and even parts differ in the opposite direction the result is taken as 0 */
            seq_t inner = (sum_pos >= sum_neg) ? (sum_pos - sum_neg) : (seq_t)0;
            term = pow2(2 * k + 1) * inner;
        }

        result += term;
    }

    return result;
}
