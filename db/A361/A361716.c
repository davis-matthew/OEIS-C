/*
 * A361716  a(n) = Sum_{k = 0..n-1} (-1)^k*binomial(n,k)^2*binomial(n-1,k).
 * https://oeis.org/A361716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k = 0..n-1} (-1)^k * binomial(n,k)^2 *
 *            binomial(n-1,k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 15, i64 n <= 29, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t res = 0;
    seq_t bn_k = 1;      /* binomial(n,0) */
    seq_t bn1_k = 1;     /* binomial(n-1,0) */

    /* Iterate k = 0 .. n-1 */
    for (idx_t k = 0; k < n; ++k) {
        /* term = bn_k^2 * bn1_k */
        seq_t term = bn_k * bn_k;
        term = term * bn1_k;

        /* add or subtract according to (-1)^k */
        res = (k & 1) ? res - term : res + term;

        /* Prepare next binomial values for k+1, unless at last k */
        if (k + 1 < n) {
            seq_t denom = (seq_t)(k + 1);
            bn_k   = bn_k * (seq_t)(n - k) / denom;          /* C(n, k+1) */
            bn1_k  = bn1_k * (seq_t)(n - 1 - k) / denom;     /* C(n-1, k+1) */
        }
    }

    return res;
}
