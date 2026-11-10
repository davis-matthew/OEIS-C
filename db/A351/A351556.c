/*
 * A351556  a(n) = gcd(n, A019565(n)).
 * https://oeis.org/A351556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = gcd(n, A019565(n)) = Product of prime(k+1) over set bits
 *            k of n such that prime(k+1) divides n
 * Range:     u8 n <= 298, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9995 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A351556_primes[64] = {
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
    seq_t result = 1;
    idx_t m = n;
    unsigned bit = 0;

    if (n == 0) return 1;
    while (m != 0) {
        if (m & 1ULL) {
            idx_t p = (idx_t)A351556_primes[bit];
            if (n % p == 0) {
                result *= (seq_t)p;
            }
        }
        m >>= 1;
        ++bit;
    }
    return result;
}
