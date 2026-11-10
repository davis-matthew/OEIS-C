/*
 * A293231  a(n) = Product_{d|n, d<n} A019565(A193231(d)).
 * https://oeis.org/A293231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Product_{d|n, d<n} A019565(A193231(d)), computed directly
 *            over the proper divisors of n
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 119,
 *            u128 n <= 359, big any n
 * Validated: standard, pass, 78 terms through n = 987 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A019565_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

static uint64_t A193231_code(uint64_t n)
{
    uint64_t res = 0, mask = 1, t = n;
    while (t) {
        if (t & 1) res ^= mask;
        mask = (mask << 1) ^ mask;
        t >>= 1;
    }
    return res;
}

static seq_t A019565_val(uint64_t n)
{
    seq_t result = 1;
    unsigned bit = 0;
    while (n != 0) {
        if (n & 1ULL) result *= (seq_t)A019565_primes[bit];
        n >>= 1;
        ++bit;
    }
    return result;
}

static seq_t term_for_divisor(uint64_t d)
{
    return A019565_val(A193231_code(d));
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t N = (uint64_t)n;
    for (uint64_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            uint64_t j = N / i;
            if (i < N) result *= term_for_divisor(i);
            if (j != i && j < N) result *= term_for_divisor(j);
        }
    }
    return result;
}
