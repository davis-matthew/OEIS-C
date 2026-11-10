/*
 * A392236  Positions of zeros in A309983.
 * https://oeis.org/A392236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th number m with A309983(m) = 0, i.e. 2*(sum of
 *            positions of 1-bits of m) = L(L+1)/2 with L = floor(log2 m);
 *            found by counting subset-sum DP per bit length and unranking
 * Range:     u8 n <= 13, u16 n <= 943, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9682 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    const uint64_t CAP = (uint64_t)1 << 62;
    uint64_t k = (uint64_t)n - 1;
    for (unsigned L = 0; ; ++L) {
        uint64_t T = (uint64_t)L * (L + 1) / 2;
        if (T & 1) continue;
        uint64_t half = T / 2;
        if (half < L) continue;
        size_t target = (size_t)(half - L);
        size_t w = target + 1;
        uint64_t *cnt = (uint64_t *)calloc((size_t)(L + 1) * w, sizeof(uint64_t));
        cnt[0] = 1;
        for (unsigned j = 0; j < L; ++j) {
            for (size_t s = 0; s < w; ++s) {
                uint64_t v = cnt[(size_t)j * w + s];
                if (s >= j) v += cnt[(size_t)j * w + s - j];
                if (v > CAP) v = CAP;
                cnt[(size_t)(j + 1) * w + s] = v;
            }
        }
        uint64_t total = cnt[(size_t)L * w + target];
        if (k < total) {
            seq_t r = (seq_t)1 << L;
            size_t s = target;
            for (unsigned j = L; j-- > 0; ) {
                uint64_t z = cnt[(size_t)j * w + s];
                if (k < z) {
                    /* bit j is 0 */
                } else {
                    k -= z;
                    r |= ((seq_t)1 << j);
                    s -= j;
                }
            }
            free(cnt);
            return r;
        }
        k -= total;
        free(cnt);
    }
}
