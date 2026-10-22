/*
 * A363973  Expansion of Sum_{k>0} k^2 * x^(4*k-1) / (1 - x^(4*k-1)).
 * https://oeis.org/A363973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d==3 mod 4} ((d+1)/4)^2
 * Range:     u8 n <= 62, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            if (d % 4 == 3) {
                seq_t val = (seq_t)(d + 1) / 4;
                sum += val * val;
            }
            /* paired divisor e = n/d */
            idx_t e = n / d;
            if (e != d && e % 4 == 3) {
                seq_t val = (seq_t)(e + 1) / 4;
                sum += val * val;
            }
        }
    }
    return sum;
}
