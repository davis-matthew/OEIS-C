/*
 * A293443  Multiplicative with a(p^e) = A019565(A193231(e)).
 * https://oeis.org/A293443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e || n} A019565(A193231(e)), a(1) = 1
 *            (multiplicative)
 * Range:     u8 n <= 767, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8777 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A293443_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

/* A193231 on small index e (e <= 63) */
static uint64_t A293443_blue(uint64_t e)
{
    uint64_t res = 0, mask = 1;
    while (e) {
        if (e & 1) res ^= mask;
        mask = (mask << 1) ^ mask;
        e >>= 1;
    }
    return res;
}

/* A019565 of index m (bits select primes) */
static seq_t A293443_sqf(uint64_t m)
{
    seq_t r = 1;
    unsigned bit = 0;
    while (m) {
        if (m & 1ULL) r *= (seq_t)A293443_primes[bit];
        m >>= 1;
        ++bit;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t t = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= t; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            result *= A293443_sqf(A293443_blue(e));
        }
    }
    if (t > 1)
        result *= A293443_sqf(A293443_blue(1));
    return result;
}
