/*
 * A394692  Triangle read by rows: T(n,k) = (1/(2k)!) * Sum_{j=0..2k} (-1)^j *
 *          binomial(2k,j) * (1+k-j)^(2n).
 * https://oeis.org/A394692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,c) where T(n,k) = (1/(2k)!) * Σ_{j=0}^{2k} (-1)^j *
 *            binomial(2k,j) * (1+k-j)^(2n) and (r,c) are the row and column
 *            of the triangle in row-major order (offset 0)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 32, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

/* Computes the n-th term of OEIS A394692. */
seq_t a(idx_t n)
{
    /* Find row r and column c such that 0 ≤ c ≤ r and
       n = r*(r+1)/2 + c.                                               */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;          /* column index */

    idx_t twok = 2 * k;

    seq_t sum_even = 0;
    seq_t sum_odd  = 0;

    for (idx_t j = 0; j <= twok; ++j) {
        /* binomial coefficient C(2k, j) */
        seq_t bin = binomial(twok, j);

        /* Base (1 + k - j), take absolute value because the exponent 2r is even. */
        int base_int = 1 + (int)k - (int)j;
        int base_abs = base_int >= 0 ? base_int : -base_int;

        seq_t pow_val = big_pow((seq_t)base_abs, (seq_t)(2 * r));
        seq_t term = bin * pow_val;

        if ((j & 1) == 0)
            sum_even += term;
        else
            sum_odd += term;
    }

    seq_t numerator = sum_even - sum_odd;

    /* Denominator = (2k)! */
    seq_t denom = 1;
    for (idx_t i = 1; i <= twok; ++i)
        denom *= (seq_t)i;

    return numerator / denom;
}
