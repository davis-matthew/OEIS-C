/*
 * A365332  The sum of divisors of the largest square dividing n.
 * https://oeis.org/A365332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum of divisors of the largest square dividing n
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t result = 1;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            int k = exp - (exp & 1);   /* exponent of the square part */
            if (k > 0) {
                seq_t term = 1;
                seq_t sum  = 1;
                for (int i = 0; i < k; ++i) {
                    term *= (seq_t)p;
                    sum  += term;
                }
                result *= sum;
            }
        }
    }
    /* Any remaining factor > 1 has exponent 1, so contributes nothing to k. */
    return result;
}
