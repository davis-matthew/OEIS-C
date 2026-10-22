/*
 * A364064  Expansion of Sum_{k>0} k * x^k / (1 - x^(3*k-2)).
 * https://oeis.org/A364064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3) * sum_{d | 3*n-2, d==1 (mod 3)} (d+2)
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1 etc. Behaviour for n<1 is irrelevant. */
    if (n < 1) return 0;
    idx_t m = 3 * n - 2;         /* 3*n-2 */
    seq_t sum = 0;

    for (idx_t d = 1; d <= m / d; ++d) {
        if (m % d == 0) {
            /* divisor d */
            if (d % 3 == 1)
                sum += ((seq_t)d + 2);

            /* complementary divisor e = m/d */
            idx_t e = m / d;
            if (e != d && e % 3 == 1)
                sum += ((seq_t)e + 2);
        }
    }
    return sum / 3;
}
