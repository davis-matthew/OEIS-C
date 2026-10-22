/*
 * A326261  E.g.f.: Sum_{n>=0} (3 + exp(n*x))^n * x^n/n!.
 * https://oeis.org/A326261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} Σ_{j=0..k} binom(n,k) · binom(k,j) · 3^{k-j} ·
 *            (j·k)^{n-k}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t binom_n_k = binomial(n, k);
        seq_t term_k   = 0;
        for (idx_t j = 0; j <= k; ++j)
        {
            seq_t binom_k_j = binomial(k, j);

            /* 3^{k-j} */
            seq_t pow3 = pow_uint(3, k - j);

            /* (j*k)^{n-k} */
            seq_t powjk;
            if (j == 0)
            {
                /* 0^0 is treated as 1, otherwise 0 */
                powjk = (n == k) ? 1 : 0;
            }
            else
            {
                powjk = pow_uint(j * k, n - k);
            }

            seq_t prod = binom_k_j * pow3 * powjk;
            term_k += prod;
        }
        sum += binom_n_k * term_k;
    }
    return sum;
}
