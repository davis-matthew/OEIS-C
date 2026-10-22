/*
 * A366087  Expansion of (1/x) * Series_Reversion( x*(1-x)/(1-x-x^4) ).
 * https://oeis.org/A366087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/4⌋} (-1)^k * binomial(n+1,k) *
 *            binomial(n-3k-1,n-4k)
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 34, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t maxk = n / 4;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t bin1 = binomial(n + 1, k);

        seq_t bin2;
        if (n < 3 * k + 1) {
            bin2 = 0;
        } else {
            idx_t num = n - 3 * k - 1;
            idx_t den = n - 4 * k;
            if (num < den)
                bin2 = 0;
            else
                bin2 = binomial(num, den);
        }

        seq_t term = bin1 * bin2;
        if (k & 1)
            term = -term;

        sum += term;
    }

    sum = sum / (seq_t)(n + 1);
    return sum;
}
