/*
 * A378463  a(n) = Sum_{k=0..floor(n/3)} binomial(n+k-1,k) *
 *          binomial(2*n-k-1,n-3*k).
 * https://oeis.org/A378463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n+k-1,k) *
 *            binomial(2*n-k-1,n-3*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* handle the special case a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    idx_t limit = n / 3;   /* floor(n/3) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= limit; ++k) {
        /* compute binomial(n + k - 1, k) */
        seq_t term1 = binomial(n + k - 1, k);
        /* compute binomial(2*n - k - 1, n - 3*k) */
        seq_t term2 = binomial(2 * n - k - 1, n - 3 * k);
        /* accumulate product */
        sum += term1 * term2;
    }
    return sum;
}
