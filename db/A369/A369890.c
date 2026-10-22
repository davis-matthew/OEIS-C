/*
 * A369890  The number of divisors of the largest divisor of n whose exponents
 *          in its prime factorization are all powers of 2.
 * https://oeis.org/A369890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (largest power of 2 ≤ e + 1) (This equals the
 *            number of divisors of the largest divisor of n whose
 *            prime‑exponent exponents are all powers of 2.)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;

    if (x <= 1)          /* a(1) = 1 */
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t e = 0;

    /* factor 2 */
    while ((x & 1ULL) == 0) {
        x >>= 1;
        ++e;
    }
    if (e > 0) {
        uint64_t f = 1;
        while (f * 2ULL <= e) f *= 2ULL;    /* largest power of 2 ≤ e */
        result *= (seq_t)(f + 1);
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            uint64_t f = 1;
            while (f * 2ULL <= e) f *= 2ULL;
            result *= (seq_t)(f + 1);
        }
    }

    /* remaining prime factor, if any */
    if (x > 1) {
        /* exponent is 1 → largest power of 2 ≤ 1 is 1 */
        result *= (seq_t)2;
    }

    return result;
}
