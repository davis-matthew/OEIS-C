/*
 * A350389  a(n) is the largest unitary divisor of n that is an exponentially
 *          odd number (A268335).
 * https://oeis.org/A350389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of p^e for all prime powers p^e in the
 *            factorisation of n * with e odd. This is the largest unitary
 *            divisor of n that is an exponentially * odd number
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n=0, return 0 */

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0)
            continue;

        uint64_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        if (e & 1U) {              /* exponent is odd */
            seq_t pow = (seq_t)1;
            for (uint64_t i = 0; i < e; ++i)
                pow *= (seq_t)p;
            result *= pow;
        }
    }

    /* If m > 1 now, it is a prime factor with exponent 1 (odd). */
    if (m > 1)
        result *= (seq_t)m;

    return result;
}
