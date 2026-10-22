/*
 * A364047  Expansion of Sum_{k>0} x^k / (1 + x^(6*k)).
 * https://oeis.org/A364047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, d==1 mod 6} (-1)^((d-1)/6)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t nn = n;
    int64_t sum = 0;            /* accumulate in signed type to avoid wrap */

    for (idx_t i = 1; i <= nn / i; ++i) {
        if (nn % i == 0) {
            /* first divisor d1 = i */
            idx_t d1 = i;
            if (d1 % 6 == 1) {
                idx_t e = (d1 - 1) / 6;
                if (e & 1) --sum;
                else       ++sum;
            }

            /* second divisor d2 = nn / i, avoid double-count when i*i == nn */
            idx_t d2 = nn / i;
            if (d2 != d1 && d2 % 6 == 1) {
                idx_t e = (d2 - 1) / 6;
                if (e & 1) --sum;
                else       ++sum;
            }
        }
    }

    return (seq_t)sum;
}
