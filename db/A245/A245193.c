/*
 * A245193  Smallest prime having in decimal representation A136333(n) as
 *          suffix.
 * https://oeis.org/A245193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest prime p = k*10^d + A136333(n) (k >= 0, d =
 *            number of digits of A136333(n)); computed by search over k
 * Range:     u8 n <= 14, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 997 (2026-11-07)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

static seq_t A245193_A136333_a(idx_t n)
{
    static const unsigned char map[4] = {1, 3, 7, 9};
    __uint128_t sum = 0;
    __uint128_t pow4 = 4;
    unsigned k = 1;

    while (sum + pow4 < (__uint128_t)n) {
        sum += pow4;
        pow4 *= 4;
        ++k;
    }

    uint64_t offset = (uint64_t)n - (uint64_t)sum;
    uint64_t off = offset - 1;

    unsigned char rev[64];
    for (unsigned i = 0; i < k; ++i) {
        rev[i] = (unsigned char)(off % 4);
        off /= 4;
    }

    seq_t result = 0;
    for (int i = (int)k - 1; i >= 0; --i) {
        result = result * 10 + map[rev[i]];
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t m = A245193_A136333_a(n);
    seq_t p10 = 1;
    seq_t t = m;
    while (t > 0) {
        p10 *= 10;
        t /= 10;
    }
    seq_t cand = m;
    while (true) {
        if (cand > 1 && is_prime_seq(cand)) return cand;
        cand += p10;
    }
}
