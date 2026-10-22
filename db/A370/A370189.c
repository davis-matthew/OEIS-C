/*
 * A370189  Imaginary part of (1 + n*i)^n, where i is the imaginary unit.
 * https://oeis.org/A370189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{j=0..floor((n-1)/2)} binomial(n,2*j+1) * n^(2*j+1) *
 *            (-1)^j
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the offset where a(0)=0 explicitly to avoid unsigned underflow */
    if (n == 0)
        return 0;

    /* Number of terms to sum: floor((n-1)/2) */
    idx_t maxj = (n - 1) / 2;

    seq_t sum = 0;
    seq_t nseq = (seq_t)n;
    seq_t pow  = nseq;                     /* n^(1) initially */

    for (idx_t j = 0; j <= maxj; ++j) {
        idx_t k = 2 * j + 1;                /* odd choose index */
        seq_t comb = binomial(n, k);        /* compute C(n,k) */
        seq_t term = comb * pow;            /* C(n,k) * n^(2*j+1) */

        if (j & 1)                          /* multiply by (-1)^j */
            term = -term;

        sum += term;

        /* Prepare pow for the next iteration: multiply by n^2 */
        if (j < maxj) {
            pow *= nseq;
            pow *= nseq;
        }
    }

    return sum;
}
