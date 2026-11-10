/*
 * A335229  Numbers expressible as the sum of two terms of A001950.
 * https://oeis.org/A335229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m such that m = A001950(i) + A001950(j) for
 *            some i,j >= 1; computed by generating A001950 terms up to a
 *            limit and marking all pairwise sums (sieve), doubling the limit
 *            until n terms are found
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A335229_build_upper(uint64_t L, size_t *cnt) {
    unsigned char *used = (unsigned char *)calloc((size_t)L + 2, 1);
    uint64_t *ups = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L / 2 + 4));
    size_t c = 0;
    uint64_t p = 1, k = 1;
    for (;;) {
        while (p <= L && used[p]) p++;
        uint64_t up = p + k;
        if (up > L) break;
        used[p] = 1;
        used[up] = 1;
        ups[c++] = up;
        k++;
    }
    free(used);
    *cnt = c;
    return ups;
}

seq_t a(idx_t n) {
    uint64_t L = 2 * (uint64_t)n + 40;
    for (;;) {
        size_t cnt;
        uint64_t *ups = A335229_build_upper(L, &cnt);
        unsigned char *sum = (unsigned char *)calloc((size_t)L + 1, 1);
        for (size_t i = 0; i < cnt; i++) {
            for (size_t j = i; j < cnt; j++) {
                uint64_t s = ups[i] + ups[j];
                if (s > L) break;
                sum[s] = 1;
            }
        }
        uint64_t seen = 0, res = 0;
        for (uint64_t m = 1; m <= L; m++) {
            if (sum[m]) {
                seen++;
                if (seen == (uint64_t)n) { res = m; break; }
            }
        }
        free(sum);
        free(ups);
        if (res) return (seq_t)res;
        L *= 2;
    }
}
