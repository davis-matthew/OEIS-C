/*
 * A363903  Expansion of Sum_{k>0} x^k / (1 - x^(4*k))^2.
 * https://oeis.org/A363903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * Σ_{d|n, d≡1 (mod 4)} (d + 3)
 * Range:     u8 n <= 764, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if ((i & 3U) == 1) {
                sum += (seq_t)i + (seq_t)3;
            }
            idx_t j = n / i;
            if (j != i && (j & 3U) == 1) {
                sum += (seq_t)j + (seq_t)3;
            }
        }
    }
    return sum / 4;
}
