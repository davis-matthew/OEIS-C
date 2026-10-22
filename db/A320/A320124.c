/*
 * A320124  Number of integer solutions to a^2 + b^2 + 2*c^2 + 3*d^2 = n.
 * https://oeis.org/A320124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #solutions of a^2 + b^2 + 2 c^2 + 3 d^2 = n, (a,b,c,d ∈
 *            ℤ)
 * Range:     u8 n <= 44, u16 n <= 7884, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Pre‑compute counts of divisors ≡1,3 (mod 4) for all numbers ≤ n.
       r2(m) = 4·(d1(m) – d3(m)) gives the number of integer pairs (a,b)
       such that a^2 + b^2 = m (m > 0), and r2(0)=1. */
    size_t sz = (size_t)(n + 1);
    uint16_t *d1 = (uint16_t *)calloc(sz, sizeof(uint16_t));
    uint16_t *d3 = (uint16_t *)calloc(sz, sizeof(uint16_t));
    if (!d1 || !d3) {
        free(d1);
        free(d3);
        return 0;
    }

    for (idx_t i = 1; i <= n; ++i) {
        uint8_t mod = (uint8_t)(i & 3);
        if (mod == 1) {
            for (idx_t j = i; j <= n; j += i) ++d1[j];
        } else if (mod == 3) {
            for (idx_t j = i; j <= n; j += i) ++d3[j];
        }
    }

    seq_t total = 0;

    for (idx_t d = 0; ; ++d) {
        __uint128_t dterm = (__uint128_t)3 * d * d;
        if (dterm > n) break;
        seq_t sd = (d == 0) ? 1 : 2;

        for (idx_t c = 0; ; ++c) {
            __uint128_t val = dterm + (__uint128_t)2 * c * c;
            if (val > n) break;
            idx_t rem = (idx_t)(n - (uint64_t)val);

            seq_t r2;
            if (rem == 0) {
                r2 = 1;
            } else {
                int32_t diff = (int32_t)d1[rem] - (int32_t)d3[rem];
                if (diff < 0) diff = 0;
                r2 = (seq_t)(4ULL * (uint32_t)diff);
            }

            seq_t sc = (c == 0) ? 1 : 2;
            total += r2 * sd * sc;
        }
    }

    free(d1);
    free(d3);
    return total;
}
