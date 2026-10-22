/*
 * A364096  Expansion of Sum_{k>0} k * x^(4*k-3) / (1 - x^(5*k-4)).
 * https://oeis.org/A364096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Sum_{d | 5*n-1, d==1 (mod 5)} (d+4)
 * Range:     u8 n <= 604, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t M = 5 * n - 1;           /* 5*n-1 is always positive for n>=1 */
    for (idx_t i = 1; i * i <= M; ++i) {
        if (M % i == 0) {
            idx_t d1 = i;
            idx_t d2 = M / i;
            if (d1 % 5 == 1) {
                result += (seq_t)(d1 + 4) / 5;
            }
            if (d2 != d1 && d2 % 5 == 1) {
                result += (seq_t)(d2 + 4) / 5;
            }
        }
    }
    return result;
}
