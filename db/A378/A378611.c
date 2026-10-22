/*
 * A378611  a(n) = Sum_{k=0..n} binomial(2*n+k-1,k) * binomial(n-1,n-k).
 * https://oeis.org/A378611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+k-1, k) * binomial(n-1, n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* a(0) = 1 */
    if (n == 0) {
        return 1;
    }

    seq_t sum = 0;
    seq_t bin1 = 1;     /* binom(2*n+0-1, 0) = 1 */
    seq_t bin2 = 0;     /* will be set for k = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update bin1: binom(2*n + k - 1, k) from previous k-1 value */
        bin1 = bin1 * ( (seq_t)(2 * n + k - 1) ) / ( (seq_t)k );

        /* Update bin2: binom(n-1, n-k) */
        if (k == 1) {
            bin2 = 1; /* binom(n-1, n-1) = 1 */
        } else {
            bin2 = bin2 * ( (seq_t)(n - (k - 1)) ) / ( (seq_t)(k - 1) );
        }

        sum += bin1 * bin2;
    }

    return sum;
}
