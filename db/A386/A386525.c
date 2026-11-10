/*
 * A386525  a(n) is the least k such that at least n terms of A063655 starting
 *          from index k are strictly decreasing.
 * https://oeis.org/A386525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least k such that A063655(k) > A063655(k+1) > ... >
 *            A063655(k+n-1), where A063655(m) = Min_{d|m} (d + m/d); computed
 *            by a segmented sieve search
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A386525_BLOCK (1u << 20)

seq_t a(idx_t n)
{
    static uint64_t cache[64];
    uint64_t want = (uint64_t)n;
    if (want < 1) want = 1;
    if (want < 64 && cache[want] != 0)
        return (seq_t)cache[want];

    uint32_t *f = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)A386525_BLOCK);
    if (!f) return (seq_t)0;

    uint64_t prev = 0, run = 0, start = 0, result = 0;
    uint64_t lo = 1;
    bool first = true;
    while (result == 0) {
        uint64_t hi = lo + A386525_BLOCK;
        for (uint32_t i = 0; i < A386525_BLOCK; i++) f[i] = 0xFFFFFFFFu;
        for (uint64_t d = 1; d * d < hi; d++) {
            uint64_t k = d * d;
            if (k < lo) {
                k = ((lo + d - 1) / d) * d;
            }
            uint64_t m = k / d;
            for (; k < hi; k += d, m++) {
                f[k - lo] = (uint32_t)(d + m);
            }
        }
        for (uint64_t k = lo; k < hi; k++) {
            uint64_t v = f[k - lo];
            if (!first && v < prev) {
                run++;
            } else {
                run = 1;
                start = k;
            }
            first = false;
            prev = v;
            if (run >= want) {
                result = start;
                break;
            }
        }
        lo = hi;
    }
    free(f);
    if (want < 64) cache[want] = result;
    return (seq_t)result;
}
