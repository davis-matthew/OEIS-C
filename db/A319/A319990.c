/*
 * A319990  a(n) = Product_{d|n, d<n} A019565(d)^[0 == d mod 3].
 * https://oeis.org/A319990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d<n, 3|d} A019565(d), where A019565(d) =
 *            Product of prime(k+1) over set bits k of d
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 71, u64 n <= 179,
 *            u128 n <= 503, big any n
 * Validated: standard, pass, 105 terms through n = 7680 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A319990_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

static seq_t A319990_A019565(idx_t n)
{
    seq_t result = 1;
    unsigned bit = 0;
    while (n != 0) {
        if (n & 1ULL) {
            result *= (seq_t)A319990_primes[bit];
        }
        n >>= 1;
        ++bit;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t d;
    for (d = 3; d <= n / 2; d += 3) {
        if (n % d == 0) {
            result *= A319990_A019565(d);
        }
    }
    return result;
}
