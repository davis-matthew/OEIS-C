/*
 * A366933  Expansion of Sum_{k>=1} k^4 * x^k/(1 - x^k)^4.
 * https://oeis.org/A366933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d^4 * binomial(n/d+2,3)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t divisor_term(idx_t n, idx_t d) {
    idx_t q = n / d;                   /* n/d integer */
    seq_t m = (seq_t)q + 2;            /* m = n/d + 2   */
    /* binomial(m,3) = m*(m-1)*(m-2)/6 */
    seq_t binom = m * (m - 1);
    binom *= (m - 2);
    binom /= 6;
    seq_t d_seq = (seq_t)d;
    seq_t d2 = d_seq * d_seq;          /* d^2   */
    seq_t d4 = d2 * d2;                /* d^4   */
    return d4 * binom;
}

seq_t a(idx_t n) {
    seq_t res = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            res += divisor_term(n, d);
            idx_t e = n / d;
            if (e != d) {
                res += divisor_term(n, e);
            }
        }
    }
    return res;
}
