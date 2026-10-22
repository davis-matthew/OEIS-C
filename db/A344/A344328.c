/*
 * A344328  Number of divisors of n^5.
 * https://oeis.org/A344328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of divisors of n^5 = ∏_{p^e||n} (5e+1)
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1U)
        return 0;               /* undefined for n < 1 */
    seq_t result = 1;
    idx_t m = n;
    idx_t p;

    for (p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint32_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            result *= (seq_t)(5 * e + 1);
        }
    }

    /* Remaining prime factor (if any) has exponent 1 */
    if (m > 1)
        result *= (seq_t)6;

    return result;
}
