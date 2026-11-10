/*
 * A258353  Where 1's occur in A212306.
 * https://oeis.org/A258353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 1 in A212306, where A212306(m) = m -
 *            Sum_{i<m, i+A212306(i)>=m} A212306(i); computed by a linear scan
 *            with a difference array
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9147 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    uint64_t N = 4 * (uint64_t)n + 100;
    for (;;) {
        uint64_t *rem = (uint64_t *)calloc((size_t)(N + 2), sizeof(uint64_t));
        if (!rem) return (seq_t)0;
        uint64_t s = 0, cnt = 0;
        for (uint64_t i = 1; i <= N; i++) {
            s -= rem[i];
            uint64_t val = i - s;
            if (val == 1) {
                cnt++;
                if (cnt == (uint64_t)n) {
                    free(rem);
                    return (seq_t)i;
                }
            }
            if (val > 0) {
                s += val;
                uint64_t e = i + val + 1;
                if (e <= N) rem[e] += val;
            }
        }
        free(rem);
        N *= 2;
    }
}
