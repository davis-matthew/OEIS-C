/*
 * A277045  Irregular triangle T(n,k) read by rows giving the number of
 *          partitions of length k such that all of the members of the
 *          partition are distinct and in A003586.
 * https://oeis.org/A277045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(m,k) = number of k-subsets of distinct 3-smooth numbers
 *            (A003586) summing to m; row m runs k = 1..max k with T(m,k) > 0.
 *            Computed by subset-sum dynamic programming (a reachability DP
 *            fixes the row lengths, and a counting DP gives the term)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static size_t A277045_gen_smooth(uint64_t lim, uint64_t *sm, size_t cap) {
    size_t cnt = 0, i, j;
    uint64_t p, q;
    for (p = 1; p <= lim; p *= 2) {
        for (q = p; q <= lim; q *= 3) {
            if (cnt < cap) sm[cnt++] = q;
            if (q > lim / 3) break;
        }
        if (p > lim / 2) break;
    }
    for (i = 1; i < cnt; i++) {
        uint64_t v = sm[i];
        j = i;
        while (j > 0 && sm[j - 1] > v) { sm[j] = sm[j - 1]; j--; }
        sm[j] = v;
    }
    return cnt;
}

seq_t a(idx_t n) {
    uint64_t M = 16;
    size_t cap = 8192;
    uint64_t *sm = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (;;) {
        size_t cnt = A277045_gen_smooth(M, sm, cap);
        size_t K = cnt, W = (size_t)M + 1, i, kk, s;
        unsigned char *reach = (unsigned char *)calloc((K + 1) * W, 1);
        reach[0] = 1;
        for (i = 0; i < cnt; i++) {
            size_t v = (size_t)sm[i];
            for (kk = i + 1; kk >= 1; kk--) {
                for (s = (size_t)M; s >= v; s--) {
                    if (reach[(kk - 1) * W + s - v]) reach[kk * W + s] = 1;
                }
            }
        }
        uint64_t pos = 0, m;
        for (m = 1; m <= M; m++) {
            uint64_t L = 0;
            for (kk = K; kk >= 1; kk--) {
                if (reach[kk * W + (size_t)m]) { L = kk; break; }
            }
            if (pos + L >= (uint64_t)n) {
                uint64_t k = (uint64_t)n - pos;
                size_t W2 = (size_t)m + 1, jj;
                seq_t *dp = mem_new(seq_t, ((size_t)k + 1) * W2);
                seq_t r;
                dp[0] = 1;
                for (i = 0; i < cnt && sm[i] <= m; i++) {
                    size_t v = (size_t)sm[i];
                    for (jj = (size_t)k; jj >= 1; jj--) {
                        for (s = (size_t)m; s >= v; s--) {
                            dp[jj * W2 + s] += dp[(jj - 1) * W2 + s - v];
                        }
                    }
                }
                r = dp[(size_t)k * W2 + (size_t)m];
                mem_free(dp);
                free(reach);
                free(sm);
                return r;
            }
            pos += L;
        }
        free(reach);
        M *= 2;
    }
}
