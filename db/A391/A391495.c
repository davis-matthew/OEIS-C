/*
 * A391495  Expansion of (g/(2 - g^2))^2, where g = 1+x*g^4 is the g.f. of
 *          A002293.
 * https://oeis.org/A391495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2*n)) * Sum_{k=1..n} k * (k*Pell(k+1) +
 *            (k+1)*Pell(k+2)) * binomial(4*n, n-k) for n > 0, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Pre‑compute Pell numbers up to Pell(n+2) */
    seq_t *pell = (seq_t *)malloc((size_t)(n + 3) * sizeof(seq_t));
    if (!pell)
        return (seq_t)0;                     /* memory failure */

    pell[0] = (seq_t)0;
    pell[1] = (seq_t)1;
    for (idx_t i = 2; i <= n + 2; ++i)
        pell[i] = (seq_t)2 * pell[i - 1] + pell[i - 2];

    /* 128‑bit accumulators for intermediate values */
    __uint128_t sum = 0;
    __uint128_t binom = 1;                    /* binomial(4*n, 0) */

    for (idx_t j = 0; j < n; ++j) {
        idx_t k = n - j;                      /* k = n‑j  (1 … n) */

        seq_t pen1 = pell[(size_t)(k + 1)];
        seq_t pen2 = pell[(size_t)(k + 2)];

        __uint128_t k128   = k;
        __uint128_t temp   = k128 * (__uint128_t)pen1 +
                             (k128 + 1) * (__uint128_t)pen2;
        __uint128_t term   = k128 * temp * binom;
        sum += term;

        if (j + 1 < n) {
            __uint128_t numerator   = binom * (((__uint128_t)4 * n) - j);
            __uint128_t denominator = j + 1;
            binom = numerator / denominator;   /* exact division */
        }
    }

    /* Final division by 2*n */
    __uint128_t denom  = ( __uint128_t)2 * n;
    seq_t result = (seq_t)(sum / denom);

    free(pell);
    return result;
}
