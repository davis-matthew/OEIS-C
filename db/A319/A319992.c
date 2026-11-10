/*
 * A319992  a(n) = Product_{d|n, d<n} A019565(d)^[2 == d mod 3].
 * https://oeis.org/A319992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d<n} A019565(d)^[2 == d mod 3]
 * Range:     u8 n <= 27, u16 n <= 69, u32 n <= 219, u64 n <= 759,
 *            u128 n <= 3079, big any n
 * Validated: standard, pass, 103 terms through n = 7882 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A319992_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

static seq_t A319992_A019565(idx_t n)
{
    seq_t result = 1;
    int bit = 0;
    while (n != 0) {
        if (n & 1ULL) result *= (seq_t)A319992_primes[bit];
        n >>= 1;
        ++bit;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    if (n <= 1) return result;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;
            if (d < n && d % 3 == 2) result *= A319992_A019565(d);
            if (q != d && q < n && q % 3 == 2) result *= A319992_A019565(q);
        }
    }
    return result;
}
