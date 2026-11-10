/*
 * A393701  a(n) is the position of the first occurrence of n in A393564.
 * https://oeis.org/A393701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m >= 0 with A393564(m) = n, where A393564(m) =
 *            (number of unit lattice squares with exactly 2 vertices strictly
 *            inside and 2 strictly outside x^2+y^2=m)/8; computed by search:
 *            doubling limit M, difference-array count of squares per m
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 46 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    uint64_t target = (uint64_t)n;
    uint64_t M = 256;
    while (1) {
        int64_t L = 1;
        while ((uint64_t)(L * L) <= M) ++L;
        L += 2;
        int32_t *diff = (int32_t *)calloc((size_t)M + 2, sizeof(int32_t));
        if (!diff) return 0;
        for (int64_t i = -L - 1; i <= L; ++i) {
            uint64_t ii  = (uint64_t)(i * i);
            uint64_t i1i = (uint64_t)((i + 1) * (i + 1));
            for (int64_t j = -L - 1; j <= L; ++j) {
                uint64_t jj  = (uint64_t)(j * j);
                uint64_t j1j = (uint64_t)((j + 1) * (j + 1));
                uint64_t d[4] = { ii + jj, i1i + jj, ii + j1j, i1i + j1j };
                for (int p = 0; p < 3; ++p)
                    for (int q = p + 1; q < 4; ++q)
                        if (d[p] > d[q]) { uint64_t t = d[p]; d[p] = d[q]; d[q] = t; }
                uint64_t lo = d[1] + 1;
                uint64_t hi = d[2] - 1;   /* d[2] >= 1 here since d[2] > d[1] or equal */
                if (d[2] == 0) continue;
                if (lo > hi || lo > M) continue;
                if (hi > M) hi = M;
                diff[lo] += 1;
                diff[hi + 1] -= 1;
            }
        }
        int64_t cur = 0;
        for (uint64_t m = 0; m <= M; ++m) {
            cur += diff[m];
            if ((uint64_t)(cur / 8) == target) {
                free(diff);
                return (seq_t)m;
            }
        }
        free(diff);
        if (M > 100000000ULL) return 0;
        M *= 2;
    }
}
