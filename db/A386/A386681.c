/*
 * A386681  Triangle read by rows: T(n,k) is the number of closed binary
 *          operations on a set of n labeled elements with a closed center of
 *          size k.
 * https://oeis.org/A386681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (*n*)th entry of the triangle read by rows where
 *            T(r,k) = binomial(r,k) * k^binomial(k+1,2) * Σ_{j=0}^{r−k}
 *            (−1)^j * binomial(r−k,j) * r^(binomial(j+1,2)+(r−k)*(r−j)) The
 *            index n is converted to row r and column k via r*(r+1)/2 ≤ n <
 *            (r+1)*(r+2)/2 and k = n − r*(r+1)/2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 20, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Determine row r and column k from the linear index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;
    idx_t m = r - k;                 /* m = r - k = n - k in the formula */

    /* Compute the outer multiplicative factors */
    seq_t bin_rk = binomial(r, k);                           /* binomial(r,k) */
    seq_t pow_k   = big_pow((seq_t)k, binomial(k + 1, 2));   /* k^C(k+1,2) */

    /* Compute the alternating sum */
    seq_t positive = 0;
    seq_t negative = 0;
    for (idx_t j = 0; j <= m; ++j)
    {
        seq_t bin_mj = binomial(m, j);

        /* Exponent: binomial(j+1,2) + m*(r-j) */
        idx_t exp1 = j * (j + 1) / 2;            /* binomial(j+1,2) */
        idx_t exp2 = m * (r - j);                /* (r-k)*(r-j) */
        seq_t exp  = (seq_t)(exp1 + exp2);

        seq_t pow_r = big_pow((seq_t)r, exp);
        seq_t term  = bin_mj * pow_r;

        if ((j & 1) == 0)
            positive += term;
        else
            negative += term;
    }
    seq_t sum = positive - negative;              /* sum is guaranteed ≥ 0 */

    seq_t result = bin_rk * pow_k * sum;
    return result;
}
