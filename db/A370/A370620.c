/*
 * A370620  Coefficient of x^n in the expansion of 1 / (1-x-x^2)^(3*n).
 * https://oeis.org/A370620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*n+k-1,k) * * *
 *            binomial(4*n-k-1,n-2*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"        /* assumes binomial(idx_t n, idx_t k) is available */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;        /* a(0) = 1 */

    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t b1_n = 3 * n + k - 1;
        idx_t b2_n = 4 * n - k - 1;
        idx_t b2_k = n - 2 * k;

        seq_t b1 = binomial(b1_n, k);
        seq_t b2 = binomial(b2_n, b2_k);

        sum += b1 * b2;
    }
    return sum;
}
