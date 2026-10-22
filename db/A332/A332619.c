/*
 * A332619  a(n) = Sum_{d|n} lcm(d, n/d) / d.
 * https://oeis.org/A332619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d / gcd(d, n/d) (OEIS A332619)
 * Range:     u8 n <= 119, u16 n <= 9949, u32 n <= 9949, u64 n <= 9949,
 *            u128 n <= 9949, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            seq_t g  = gcd((seq_t)d1, (seq_t)d2);
            sum += (seq_t)d1 / g;
            if (d1 != d2)
                sum += (seq_t)d2 / g;
        }
    }
    return sum;
}
