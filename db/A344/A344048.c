/*
 * A344048  T(n, k) = n! * [x^n] exp(k * x/(1 - x))/(1 - x). Triangle read by
 *          rows, T(n, k) for 0 <= k <= n.
 * https://oeis.org/A344048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, T(r,k) = r! * Sum_{j=0..r} binomial(r,j) * k^j / j!.
 *            The sequence is the rows of the triangle read left‑to‑right,
 *            starting with row r = 0
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* determine row r (0‑based) and column k so that
       n = r*(r+1)/2 + k, 0 <= k <= r */
    double nd = (double)n;
    idx_t r = (idx_t)((sqrt(8.0*nd + 1.0) - 1.0) / 2.0);

    /* correct possible off‑by‑one due to rounding */
    while ((r+1)*(r+2)/2 <= n) ++r;
    while (r*(r+1)/2 > n) --r;
    idx_t k = n - r*(r+1)/2;

    /* compute factorial(r) iteratively (sequential multiplications) */
    seq_t fact_r = 1;
    for (idx_t i = 1; i <= r; ++i)
        fact_r *= (seq_t)i;

    /* compute the sum */
    seq_t sum = 0;
    seq_t binom = 1;          /* binomial(r,0) */
    seq_t kpow = 1;           /* k^0   */
    seq_t fact_j = 1;         /* j! when j=0 */

    for (idx_t j = 0; j <= r; ++j)
    {
        /* term = binom(r,j) * k^j * fact_r / j! */
        seq_t term = binom * kpow;
        term = term * fact_r;
        term = term / fact_j;
        sum += term;

        /* update for next j, except after the last iteration */
        if (j < r) {
            binom = (binom * (seq_t)(r - j)) / (seq_t)(j + 1);
            kpow = kpow * (seq_t)k;
            fact_j = fact_j * (seq_t)(j + 1);
        }
    }

    return sum;
}
