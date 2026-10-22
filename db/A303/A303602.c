/*
 * A303602  a(n) = Sum_{k = 0..n} k*binomial(2*n+1, k).
 * https://oeis.org/A303602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(4^n - binomial(2*n, n))/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* two_n = 2*n */
    idx_t two_n = n * 2;

    /* 4^n as pow2(2*n) */
    seq_t pow4 = pow2(two_n);

    /* binomial(2*n, n) */
    seq_t binom = binomial(two_n, n);

    /* diff = 4^n - binomial(2*n, n) */
    seq_t diff = pow4 - binom;

    /* two_n_plus_one = 2*n + 1 */
    seq_t two_n_plus_one = (seq_t)n * 2 + 1;

    /* numerator = (2*n+1) * diff */
    seq_t numerator = two_n_plus_one * diff;

    /* result = numerator / 2 */
    seq_t result = numerator / 2;

    return result;
}
