/*
 * A247611  a(n) = Sum_{k=0..6} binomial(12,k)*binomial(n,k).
 * https://oeis.org/A247611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..6} binomial(12,k)*binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Coefficients binomial(12,k) for k = 0..6 */
    static const seq_t coeff[7] = {1, 12, 66, 220, 495, 792, 924};

    seq_t sum = 0;

    for (idx_t k = 0; k <= 6; ++k) {
        if (k == 0) {
            /* binomial(n,0) == 1 */
            sum += coeff[0];
            continue;
        }

        if (k > n) {
            /* binomial(n,k) == 0 when n < k */
            continue;
        }

        seq_t bin = 1;
        /* Compute binomial(n,k) multiplicatively: n*(n-1)*...*(n-k+1)/k! */
        for (idx_t i = 1; i <= k; ++i) {
            /* (n - k + i) is safe because k <= n */
            bin = (bin * (seq_t)(n - k + i)) / (seq_t)i;
        }

        sum += bin * coeff[k];
    }

    return sum;
}
