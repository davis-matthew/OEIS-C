/*
 * A334080  Number of Pythagorean triples among the divisors of 60*n.
 * https://oeis.org/A334080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Pythagorean triples among the divisors of 60*n
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Integer square root of a 64‑bit unsigned integer. */
static inline uint64_t isqrt64(uint64_t x)
{
    uint64_t low = 0, high = 1ULL << 32;   /* high is larger than sqrt(2^64-1) */
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* M = 60 * n */
    uint64_t M = 60ULL * (uint64_t)n;

    /* Factor M */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(M, pr, ex, &cnt);

    /* Generate all divisors of M */
    uint64_t divs[1000];    /* 600 000 has fewer than 1200 divisors */
    size_t dcnt = 1;
    divs[0] = 1ULL;
    for (size_t pi = 0; pi < cnt; ++pi) {
        uint64_t p = pr[pi];
        int e = (int)ex[pi];
        size_t d_before = dcnt;
        uint64_t mult = 1ULL;
        for (int k = 1; k <= e; ++k) {
            mult *= p;
            for (size_t j = 0; j < d_before; ++j) {
                divs[dcnt++] = divs[j] * mult;
            }
        }
    }

    /* Count Pythagorean triples */
    size_t triples = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t a = divs[i];
        for (size_t j = i + 1; j < dcnt; ++j) {
            uint64_t b = divs[j];
            uint64_t c2 = a * a + b * b;
            uint64_t c  = isqrt64(c2);
            if (c * c == c2 && M % c == 0) {
                ++triples;
            }
        }
    }
    return (seq_t)triples;
}
