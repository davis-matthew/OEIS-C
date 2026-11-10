/*
 * A332825  a(n) = A019565(phi(n)).
 * https://oeis.org/A332825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A019565(phi(n)), where A019565(m) = Product_{k in bits of
 *            m} prime(k+1)
 * Range:     u8 n <= 28, u16 n <= 126, u32 n <= 2038, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 94 terms through n = 6597 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A332825_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

seq_t a(idx_t n)
{
    idx_t phi = 1;
    idx_t m = n;
    idx_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            m /= p;
            phi *= (p - 1);
            while (m % p == 0) {
                m /= p;
                phi *= p;
            }
        }
    }
    if (m > 1)
        phi *= (m - 1);

    seq_t result = 1;
    unsigned bit = 0;
    while (phi != 0 && bit < 64) {
        if (phi & 1u)
            result *= (seq_t)A332825_primes[bit];
        phi >>= 1;
        ++bit;
    }
    return result;
}
