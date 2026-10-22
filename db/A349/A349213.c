/*
 * A349213  a(n) = Sum_{d|n} n^((d+1) mod 2).
 * https://oeis.org/A349213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} n^((d+1) mod 2) (i.e., add n for even d,
 *            otherwise 1)
 * Range:     u8 n <= 47, u16 n <= 2159, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            /* add contribution for divisor d */
            total += ((d & 1ULL) ? 1 : (seq_t)n);
            /* if d and e are distinct, add contribution for e */
            if (e != d) {
                total += ((e & 1ULL) ? 1 : (seq_t)n);
            }
        }
    }
    return total;
}
