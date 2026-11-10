/*
 * A292265  A multiplicative encoding (compressed) for the exponents of 2
 *          obtained when using Shevelev's algorithm for computing A002326.
 * https://oeis.org/A292265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_i A019565(v_i), where with N=2n+1, t=1, repeat {t
 *            += N; v = A007814(t); t /= 2^v; record v} until t = 1
 *            (Shevelev's algorithm); then A048675(a(n)) = A002326(n)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 25, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 63 terms through n = 1008 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A292265_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

static seq_t A292265_A019565(unsigned k)
{
    seq_t r = 1;
    unsigned bit = 0;
    while (k != 0 && bit < 64) {
        if (k & 1u) r *= (seq_t)A292265_primes[bit];
        k >>= 1;
        ++bit;
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n * 2 + 1;
    uint64_t t = 1;
    seq_t result = 1;
    for (;;) {
        t += N;
        unsigned v = 0;
        while ((t & 1u) == 0) {
            t >>= 1;
            ++v;
        }
        result *= A292265_A019565(v);
        if (t == 1) break;
    }
    return result;
}
