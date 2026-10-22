/*
 * A326090  E.g.f.: Sum_{n>=0} (1 + exp(n*x))^n * x^n/n!.
 * https://oeis.org/A326090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binom(n,k) * Sum_{j=0..k} binom(k,j) *
 *            (j*k)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* binomial coefficient binom(n,k) */
        seq_t bin_nk = binom(n, k);
        seq_t inner  = 0;
        idx_t exp = n - k;                /* exponent (n-k) */
        for (idx_t j = 0; j <= k; ++j)
        {
            seq_t bin_kj = binom(k, j);
            seq_t base  = (seq_t)(j * k); /* (j*k) */
            seq_t p;
            /* Handle 0^0 explicitly: 0^0 = 1, 0^m (m>0) = 0 */
            if (j == 0)
            {
                if (exp == 0)
                    p = 1;          /* 0^0 */
                else
                    p = 0;          /* 0^m, m>0 */
            }
            else
            {
                p = big_pow(base, (seq_t)exp);
            }
            inner += bin_kj * p;
        }
        total += bin_nk * inner;
    }
    return total;
}
