/*
 * A363865  a(n) = A143007(3*n,n).
 * https://oeis.org/A363865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n,n-k)^2*binomial(3*n+k,k)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* precompute 3*n once */
    idx_t three_n = n + n + n;

    /* B = binomial(3*n, n-k), starting with k = 0  -> binomial(3*n, n) */
    seq_t B = binomial(three_n, n);
    /* C = binomial(3*n + k, k), starting with k = 0  -> 1 */
    seq_t C = 1;

    for (idx_t k = 0; k <= n; ++k) {
        /* term = B^2 * C^2 */
        seq_t term = B * B * C * C;
        sum += term;

        if (k == n) break;

        /* update B for k+1: B = B * (n-k) / (2*n + k + 1)   (exact division) */
        seq_t numB = (seq_t)(n - k);
        seq_t denB = (seq_t)(2 * n + k + 1);
        B = (B * numB) / denB;

        /* update C for k+1: C = C * (3*n + k + 1) / (k+1) (exact division) */
        seq_t numC = (seq_t)(three_n + k + 1);
        seq_t denC = (seq_t)(k + 1);
        C = (C * numC) / denC;
    }
    return sum;
}
