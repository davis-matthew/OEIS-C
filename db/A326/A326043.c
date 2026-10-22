/*
 * A326043  Multiplicative with a(p^e) = floor[((e-1)+sigma(p^e)) / e].
 * https://oeis.org/A326043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = multiplicative with a(p^e) = floor(((e-1)+sigma(p^e)) /
 *            e), where sigma(p^e) = 1 + p + p^2 + ... + p^e
 * Range:     u8 n <= 137, u16 n <= 9935, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* compute sigma(p^e) */
            seq_t sigma = 1;
            seq_t p_pow = 1;
            for (idx_t k = 1; k <= e; ++k) {
                p_pow *= (seq_t)p;
                sigma += p_pow;
            }
            seq_t num = (seq_t)(e - 1) + sigma;
            seq_t ap = num / (seq_t)e;
            result *= ap;
        }
    }
    /* if a prime factor remains */
    if (m > 1) {
        seq_t sigma = 1 + (seq_t)m;     /* e = 1, sigma = 1 + m */
        seq_t ap = sigma;               /* a(p) = p + 1 */
        result *= ap;
    }
    return result;
}
