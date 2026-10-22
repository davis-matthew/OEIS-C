/*
 * A363978  Expansion of Sum_{k>0} x^(3*k) / (1 - x^(4*k))^3.
 * https://oeis.org/A363978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d≡3 (mod 4)} binomial((d+1)/4+1, 2)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t d;
    for (d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            if ((d & 3) == 3) {
                idx_t k = (d + 1) >> 2;  /* k = (d+1)/4 */
                sum += (seq_t)k * (k + 1) / 2;
            }
            /* corresponding divisor n/d */
            idx_t d2 = n / d;
            if (d2 != d && (d2 & 3) == 3) {
                idx_t k = (d2 + 1) >> 2;
                sum += (seq_t)k * (k + 1) / 2;
            }
        }
    }
    return sum;
}
