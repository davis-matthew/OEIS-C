/*
 * A370626  Coefficient of x^n in the expansion of 1 / (1-x-x^3)^(2*n).
 * https://oeis.org/A370626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n+k-1, k) * *
 *            binomial(3*n-2*k-1, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the closed form without relying on any external libraries. */

#include <stdint.h>

/* The sequence element type and index type are provided by the build */
extern seq_t a(idx_t n);

/* Internal multiplicative binomial implementation (exact for all supported seq_t). */
static seq_t my_binomial(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        /* multiply first, then divide – division is exact */
        res = (res * (seq_t)(N - K + i)) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;         /* a(0) = 1 */

    seq_t sum = 0;
    idx_t kmax = n / 3;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t b1 = my_binomial(2 * n + k - 1, k);
        seq_t b2 = my_binomial(3 * n - 2 * k - 1, n - 3 * k);
        sum += b1 * b2;
    }
    return sum;
}
