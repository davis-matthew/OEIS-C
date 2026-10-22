/*
 * A385049  The sum of the unitary divisors of n that are biquadratefree
 *          numbers (A046100).
 * https://oeis.org/A385049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative with a(p^e) = p^e + 1 for e ≤ 3, a(p^e) = 1 for e
 *            ≥ 4
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* offset is 1; values for n = 0 are undefined, return 0 */
    if (n == 0) return 0;

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* handle prime 2 separately */
    uint64_t e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        e++;
    }
    if (e > 0 && e <= 3) {
        seq_t pow = 1;
        for (uint64_t i = 0; i < e; i++) pow *= (seq_t)2;
        result *= (pow + 1);
    }

    /* trial division by odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (exp <= 3) {
                seq_t pow = 1;
                for (uint64_t i = 0; i < exp; i++) pow *= (seq_t)p;
                result *= (pow + 1);
            }
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        /* exponent is 1, which is ≤ 3 */
        seq_t pow = (seq_t)m;   /* p^1 == p */
        result *= (pow + 1);
    }

    return result;
}
