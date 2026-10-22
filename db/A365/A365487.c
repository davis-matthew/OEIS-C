/*
 * A365487  The number of divisors of the largest cube dividing n.
 * https://oeis.org/A365487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e||n} (3*(e/3)+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    uint64_t m = (uint64_t)n;
    seq_t result = 1;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            result *= (seq_t)((3 * (e / 3)) + 1);
        }
    }
    /* remaining prime factor (if any) has exponent 1, factor = 1; no change */
    return result;
}
