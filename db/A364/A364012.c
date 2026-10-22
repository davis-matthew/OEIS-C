/*
 * A364012  Expansion of Sum_{k>0} k * x^k / (1 + x^(3*k)).
 * https://oeis.org/A364012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, n/d≡1 (mod 3)} { +d if (n/d) odd, −d if (n/d)
 *            even }
 * Range:     u8 n <= 216, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    long long sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            idx_t m = n / d;
            if (m % 3 == 1) {
                if (m & 1)
                    sum += (long long)d;
                else
                    sum -= (long long)d;
            }
            /* second divisor m, if distinct */
            if (d != m) {
                idx_t d2 = m;
                idx_t m2 = d;
                if (m2 % 3 == 1) {
                    if (m2 & 1)
                        sum += (long long)d2;
                    else
                        sum -= (long long)d2;
                }
            }
        }
    }
    return (seq_t)sum;
}
