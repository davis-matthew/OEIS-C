/*
 * A300832  a(n) = Product_{d|n} A019565(d)^[moebius(n/d) = -1].
 * https://oeis.org/A300832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} A019565(d)^[moebius(n/d) = -1]
 * Range:     u8 n <= 27, u16 n <= 29, u32 n <= 179, u64 n <= 509,
 *            u128 n <= 2309, big any n
 * Validated: standard, pass, 90 terms through n = 7604 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A300832_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

static seq_t A019565_a(idx_t n)
{
    seq_t result = 1;
    idx_t bit = 0;
    while (n != 0) {
        if (n & 1ULL) {
            result *= (seq_t)A300832_primes[bit];
        }
        n >>= 1;
        ++bit;
    }
    return result;
}

static int A300832_mobius(idx_t m)
{
    int s = 1;
    idx_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            s = -s;
        }
    }
    if (m > 1) s = -s;
    return s;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t d;
    for (d = 1; d <= n; d++) {
        if (n % d == 0) {
            if (A300832_mobius(n / d) == -1) {
                result *= A019565_a(d);
            }
        }
    }
    return result;
}
