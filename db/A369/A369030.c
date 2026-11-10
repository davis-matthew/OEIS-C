/*
 * A369030  Exponential of Mangoldt function permuted by A163511
 *          ("Doudna-permutation mirrored").
 * https://oeis.org/A369030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = A368900(1 + A054429(n)), where
 *            A368900(m) = prime(j+1) if m-1 = 2^i - 2^j, else 1. This equals
 *            A014963(A163511(n))
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9399 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A369030_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* x = A054429(n): reverse the block of 2^k numbers containing n */
    int k = 63 - __builtin_clzll((unsigned long long)n);
    uint64_t start = ((uint64_t)1) << k;
    uint64_t last = (start << 1) - 1;
    uint64_t x = last - ((uint64_t)n - start);

    /* x = 2^i - 2^j ?  Then result is prime(j+1), else 1 */
    unsigned j = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++j;
    }
    if ((x & (x + 1)) == 0) {
        return (seq_t)A369030_primes[j];
    }
    return (seq_t)1;
}
