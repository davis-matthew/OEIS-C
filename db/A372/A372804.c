/*
 * A372804  Decimal expansion of Sum_{k >= 1, A034887(k) even} 1/2^k.
 * https://oeis.org/A372804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) * Sum_{k>=1, A034887(k) even} 1/2^k) mod
 *            10, where A034887(k) = floor(k*log10(2))+1; computed with a big
 *            binary fixed-point fraction, multiplying repeatedly by 10
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9654 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned A034887_a(uint64_t k)
{
    const unsigned __int128 NUM = 301029995663981195ULL;
    const unsigned __int128 DEN = 1000000000000000000ULL;
    unsigned __int128 prod = (unsigned __int128)k * NUM;
    return (unsigned)(prod / DEN) + 1;
}

seq_t a(idx_t n)
{
    uint64_t N = ((uint64_t)n + 1 + 30) * 10 / 3 + 64;
    size_t W = (size_t)((N + 31) / 32);
    uint32_t *w = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint64_t k;
    uint64_t t;
    uint32_t digit = 0;
    size_t i;

    if (!w) return 0;
    for (k = 1; k <= N; k++) {
        if ((A034887_a(k) & 1u) == 0) {
            w[(k - 1) / 32] |= (uint32_t)1 << (31 - (unsigned)((k - 1) % 32));
        }
    }
    for (t = 0; t <= (uint64_t)n; t++) {
        uint64_t carry = 0;
        for (i = W; i-- > 0;) {
            uint64_t v = (uint64_t)w[i] * 10 + carry;
            w[i] = (uint32_t)v;
            carry = v >> 32;
        }
        digit = (uint32_t)carry;
    }
    free(w);
    return (seq_t)digit;
}
