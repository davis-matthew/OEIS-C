/*
 * A395457  Triangle read by rows: T(n,k) = (1/(2k)!) * Sum_{j=0..2k} (-1)^j *
 *          binomial(2k,j) * (3+k-j)^(2n).
 * https://oeis.org/A395457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the triangle T(r,k) read by rows, where r = largest
 *            integer such that r*(r+1)/2 <= n, k = n - r*(r+1)/2. T(r,k) =
 *            (1/(2k)!) * Σ_{j=0}^{2k} (-1)^j binom(2k,j) (3+k-j)^(2r)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 29, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* determine r and k */
    idx_t r = 0;
    /* find smallest r with (r+1)(r+2)/2 > n */
    while (((r + 1) * (r + 2)) / 2 <= n) r++;
    idx_t k = n - r * (r + 1) / 2;

    /* denominator (2k)! */
    seq_t denom = 1;
    for (idx_t i = 2; i <= 2 * k; ++i) denom *= i;

    /* accumulate positive and negative parts separately */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    idx_t exp = 2 * r;   /* 2n */

    for (idx_t j = 0; j <= 2 * k; ++j) {
        /* binomial coefficient C(2k, j) */
        seq_t binom = binomial(2 * k, j);

        /* base = |3 + k - j| */
        int64_t tmp = (int64_t)3 + (int64_t)k - (int64_t)j;
        seq_t base = (seq_t)(tmp >= 0 ? tmp : -tmp);

        /* compute base^exp by repeated multiplication */
        seq_t pow_val = 1;
        for (idx_t e = 0; e < exp; ++e) pow_val *= base;

        seq_t term = binom * pow_val;
        if (j & 1ULL)
            sum_neg += term;
        else
            sum_pos += term;
    }

    /* numerator = sum_pos - sum_neg (guaranteed non‑negative) */
    seq_t numerator = sum_pos - sum_neg;

    return numerator / denom;
}
