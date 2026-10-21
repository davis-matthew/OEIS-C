/*
 * A284361  a(n) = Sum_{d|n, d = 0, 1, or 4 mod 5} d.
 * https://oeis.org/A284361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d≡0,1,4 (mod 5)} d
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, used by idx_t in the build */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return sum;          /* offset is 1; 0 is never queried */

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t other = n / d;
            if (d % 5 == 0 || d % 5 == 1 || d % 5 == 4)
                sum += (seq_t)d;
            if (other != d) {
                if (other % 5 == 0 || other % 5 == 1 || other % 5 == 4)
                    sum += (seq_t)other;
            }
        }
    }
    return sum;
}
