/*
 * A382708  Number of triples (i,j,k), 1 <= i < j < k <= n such that
 *          A064413(i) < A064413(k) < A064413(j).
 * https://oeis.org/A382708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples i<j<k<=n with E(i)<E(k)<E(j), E =
 *            A064413 (EKG sequence); computed by generating EKG terms then an
 *            O(n^2) pattern count
 * Range:     u8 n <= 19, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: standard, pass, 79 terms through n = 100 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;
    size_t N = (size_t)n;
    size_t lim = 4 * N + 100;
    uint32_t *e = (uint32_t *)malloc((N + 2) * sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc(lim + 2, 1);
    uint32_t *ptr = (uint32_t *)calloc(lim + 2, sizeof(uint32_t));
    e[1] = 1; e[2] = 2;
    used[1] = 1; used[2] = 1;
    for (size_t t = 3; t <= N; t++) {
        uint32_t x = e[t - 1];
        uint32_t best = 0xFFFFFFFFu;
        for (uint32_t p = 2; (uint64_t)p * p <= x || x > 1; p++) {
            if (x % p) {
                if ((uint64_t)p * p > x) { p = x; } else continue;
            }
            while (x % p == 0) x /= p;
            uint32_t c = ptr[p] ? ptr[p] : p;
            while (used[c]) c += p;
            ptr[p] = c;
            if (c < best) best = c;
            if (x == 1) break;
        }
        used[best] = 1;
        e[t] = best;
    }
    size_t M = 0;
    for (size_t t = 1; t <= N; t++) if (e[t] > M) M = e[t];
    uint32_t *cnt = (uint32_t *)calloc(M + 2, sizeof(uint32_t));
    uint64_t total = 0;
    for (size_t j = 1; j <= N; j++) {
        /* cnt[v] = number of i<j with e[i] <= v */
        if (j > 1) {
            for (size_t v = e[j - 1]; v <= M; v++) cnt[v]++;
        }
        uint32_t aj = e[j];
        for (size_t k = j + 1; k <= N; k++) {
            uint32_t ak = e[k];
            if (ak < aj) total += cnt[ak - 1];
        }
    }
    free(cnt); free(ptr); free(used); free(e);
    return (seq_t)total;
}
