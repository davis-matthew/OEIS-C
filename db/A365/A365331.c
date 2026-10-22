/*
 * A365331  The number of divisors of the largest square dividing n.
 * https://oeis.org/A365331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = A000005(A008833(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    idx_t m = n;
    seq_t result = 1;
    idx_t e, p;

    for (p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            seq_t mult = (seq_t)(e + 1 - (e & 1));
            result *= mult;
        }
    }

    /* any remaining prime factor has exponent 1 and contributes 1,
       so the product stays unchanged */
    return result;
}
