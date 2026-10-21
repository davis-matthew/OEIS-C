/*
 * A279395  a(n) = Sum_{ d >= 1, d divides n} (-1)^(n-d)*d^4.
 * https://oeis.org/A279395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} (-1)^(n-d) * d^4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 251, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a signed 64‑bit accumulator so we can add negative
       contributions without overflow.  For n ≤ 10⁴ the maximum
       |d^4| is 10¹⁶ < 2⁶³, so long long is safe for all
       intermediate values. */
    long long sum = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if ((n % i) == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* d1^4 */
            long long d1sq4 = (long long)d1 * d1 * d1 * d1;
            long long term1 = ((n - d1) & 1) ? -d1sq4 : d1sq4;
            sum += term1;

            if (d1 != d2) {
                /* d2^4 */
                long long d2sq4 = (long long)d2 * d2 * d2 * d2;
                long long term2 = ((n - d2) & 1) ? -d2sq4 : d2sq4;
                sum += term2;
            }
        }
    }
    return (seq_t)sum;
}
