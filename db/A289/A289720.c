/*
 * A289720  a(n) = 1 + n*binomial(2*n,n) - n^2*(n^2 - 2*n + 3)/2.
 * https://oeis.org/A289720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n*binomial(2*n,n) - n²*(n² - 2*n + 3)/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Convert index to the integer type used for values. */
    seq_t N = (seq_t)n;

    /* Compute binomial(2n, n). */
    seq_t bin = binomial(2 * n, n);

    /* n * binomial(2n, n). */
    seq_t prod = N * bin;

    /* n². */
    seq_t n2 = N * N;

    /* (n² - 2*n + 3). */
    seq_t inner = n2 - (seq_t)2 * N + (seq_t)3;

    /* n² * (n² - 2*n + 3) / 2. */
    seq_t sub = (n2 * inner) / (seq_t)2;

    /* a(n) = 1 + n*binomial(2n,n) - n²*(n² - 2*n + 3)/2. */
    seq_t result = (seq_t)1 + prod - sub;

    return result;
}
