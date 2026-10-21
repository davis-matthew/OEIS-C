/*
 * A280078  Partial products of A007429 (Sum_{d|n} sigma(d)).
 * https://oeis.org/A280078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} Sum_{d|i} sigma(d)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t size = n + 1;

    /* sigma[i] will hold the sum of divisors of i */
    seq_t *sigma = (seq_t*)calloc((size_t)size, sizeof(seq_t));
    if (!sigma)
        return 0;

    for (idx_t d = 1; d <= n; ++d) {
        seq_t val = (seq_t)d;
        for (idx_t m = d; m <= n; m += d) {
            sigma[m] += val;
        }
    }

    /* S[i] = sum_{d|i} sigma(d) (i.e. the convolution sigma*1) */
    seq_t *S = (seq_t*)calloc((size_t)size, sizeof(seq_t));
    if (!S) {
        free(sigma);
        return 0;
    }

    for (idx_t d = 1; d <= n; ++d) {
        seq_t sd = sigma[d];
        for (idx_t m = d; m <= n; m += d) {
            S[m] += sd;
        }
    }

    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i) {
        prod *= S[i];
    }

    free(sigma);
    free(S);
    return prod;
}
