/*
 * A293214  a(n) = Product_{d|n, d<n} A019565(d).
 * https://oeis.org/A293214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Product_{d|n, d<n} A019565(d), where A019565(d) =
 *            Product_{k in set bits of d} prime(k+1)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 59, u64 n <= 119,
 *            u128 n <= 359, big any n
 * Validated: standard, pass, 78 terms through n = 1009 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A293214_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

static seq_t A293214_A019565(idx_t n)
{
    seq_t result = 1;
    unsigned bit = 0;
    while (n != 0) {
        if (n & 1ULL) {
            result *= (seq_t)A293214_primes[bit];
        }
        n >>= 1;
        ++bit;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t i;
    for (i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            idx_t c = n / i;
            if (i < n) {
                result *= A293214_A019565(i);
            }
            if (c != i && c < n) {
                result *= A293214_A019565(c);
            }
        }
    }
    return result;
}
