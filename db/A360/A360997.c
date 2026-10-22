/*
 * A360997  Multiplicative with a(p^e) = e + 3.
 * https://oeis.org/A360997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e || n} (e + 3); a(1) = 1
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* factor n by trial division */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            result *= (seq_t)(exp + 3);
        }
    }

    /* any remaining factor is a prime > sqrt(original n) */
    if (m > 1) {
        result *= (seq_t)(1 + 3);   /* exponent 1 => 1+3 */
    }

    return result;
}
