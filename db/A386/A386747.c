/*
 * A386747  a(n) = n^2*sigma_4(n).
 * https://oeis.org/A386747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^2 * σ₄(n) where σ₄(n) = Σ_{d|n} d^4 and a(0)=0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 39, u64 n <= 1607,
 *            u128 n <= 9000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* a(0) = 0 by definition */

    seq_t sigma4 = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t d1   = (seq_t)d;
            seq_t d1_4 = d1 * d1 * d1 * d1;   /* d^4 */
            sigma4 += d1_4;

            idx_t e = n / d;
            if (e != d) {
                seq_t d2   = (seq_t)e;
                seq_t d2_4 = d2 * d2 * d2 * d2; /* e^4 */
                sigma4 += d2_4;
            }
        }
    }

    seq_t n_sq = (seq_t)n * (seq_t)n;
    return n_sq * sigma4;
}
