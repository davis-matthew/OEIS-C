/*
 * A370102  a(n) = Sum_{k=0..n} binomial(4*n,k) * binomial(5*n-k-1,n-k).
 * https://oeis.org/A370102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n,k) * binomial(5*n-k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 because the second binomial would use a negative
       upper index. */
    if (n == 0) return 1;

    /* c1_k = binomial(4*n, k);  c2_k = binomial(5*n-k-1, n-k)   */
    seq_t c1 = 1;
    seq_t c2 = binomial(5*n-1, n);

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        sum += c1 * c2;

        if (k == n) break;

        /* update c1: c1_{k+1} = c1_k * (4*n - k) / (k+1)   */
        seq_t num1 = (seq_t)(4*n - k);
        seq_t den1 = (seq_t)(k + 1);
        c1 = (c1 * num1) / den1;

        /* update c2: c2_{k+1} = c2_k * (n - k) / (5*n - k - 1) */
        seq_t num2 = (seq_t)(n - k);
        seq_t den2 = (seq_t)(5*n - k - 1);
        c2 = (c2 * num2) / den2;
    }

    return sum;
}
