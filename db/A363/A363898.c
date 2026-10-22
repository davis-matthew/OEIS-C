/*
 * A363898  Expansion of Sum_{k>0} k * x^(2*k) / (1 - x^(5*k)).
 * https://oeis.org/A363898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, n/d == 2 mod 5} d
 * Range:     u8 n <= 377, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t total = 0;
    for (idx_t k = 1; k <= n / k; ++k) {
        if (n % k == 0) {
            idx_t d1 = k;
            idx_t d2 = n / k;
            /* check divisor d1 */
            if (d2 % 5 == 2) {
                total += (seq_t)d1;
            }
            /* check divisor d2 (distinct from d1) */
            if (d1 % 5 == 2 && d1 != d2) {
                total += (seq_t)d2;
            }
        }
    }
    return total;
}
