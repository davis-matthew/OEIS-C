/*
 * A379812  a(n) = sigma_1(n) * sigma_2(n).
 * https://oeis.org/A379812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma_1(n) * sigma_2(n)
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 959, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sigma1 = 0;
    seq_t sigma2 = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            sigma1 += (seq_t)i;
            sigma1 += (seq_t)j;
            sigma2 += (seq_t)i * (seq_t)i;
            sigma2 += (seq_t)j * (seq_t)j;
            if (i == j) {
                sigma1 -= (seq_t)i;       /* remove duplicate addition */
                sigma2 -= (seq_t)i * (seq_t)i;
            }
        }
    }
    return sigma1 * sigma2;
}
