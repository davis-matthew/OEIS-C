/*
 * A363971  Expansion of Sum_{k>0} k^2 * x^(3*k-1) / (1 - x^(3*k-1)).
 * https://oeis.org/A363971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d==2 mod 3} ((d+1)/3)^2
 * Range:     u8 n <= 46, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* first divisor d1 = i */
            idx_t d1 = i;
            if (d1 % 3 == 2) {
                seq_t t = ((seq_t)d1 + 1) / 3;
                sum += t * t;
            }
            /* second divisor d2 = n / i */
            idx_t d2 = n / i;
            if (d2 != d1 && d2 % 3 == 2) {
                seq_t t = ((seq_t)d2 + 1) / 3;
                sum += t * t;
            }
        }
    }
    return sum;
}
