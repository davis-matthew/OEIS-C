/*
 * A395418  For any n with Hamming weight h > 0, reverse each chunk of h bits
 *          in the binary expansion of n (possibly with leading 0's); a(0) =
 *          0.
 * https://oeis.org/A395418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse each chunk of h bits of the binary expansion of
 *            n, * where h = popcount(n). a(0) = 0
 * Range:     u8 n <= 46, u16 n <= 766, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 80 terms through n = 7921 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* reverse the lowest h bits of x (h <= 64) */
static inline uint64_t rev_bits64(uint64_t x, unsigned h)
{
    uint64_t rev = 0;
    for (unsigned i = 0; i < h; ++i) {
        rev = (rev << 1) | (x & 1U);
        x >>= 1;
    }
    return rev;
}

static inline unsigned int popcount_u64(uint64_t x)
{
    return __builtin_popcountll(x);
}

static inline unsigned int bitlen_u64(uint64_t x)
{
    /* x > 0 */
    return 64 - __builtin_clzll(x);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint64_t n64 = (uint64_t)n;
    unsigned h = popcount_u64(n64);          /* h > 0 */
    unsigned L = bitlen_u64(n64);           /* bit length of n */
    unsigned K = (L + h - 1) / h;            /* number of chunks */
    unsigned M = K * h;                      /* total bits after padding */

    seq_t result = 0;
    uint64_t mask = (h == 64) ? UINT64_MAX : ((1ULL << h) - 1);

    for (unsigned cj = 0; cj < K; ++cj) {
        unsigned shift = M - (cj + 1) * h;
        uint64_t chunk = (shift < 64) ? ((n64 >> shift) & mask) : 0;
        uint64_t rev = rev_bits64(chunk, h);
        if (cj == 0)
            result = (seq_t)rev;
        else
            result = (result << h) | rev;
    }

    return result;
}
