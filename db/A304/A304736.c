/*
 * A304736  Restricted growth sequence transform of A048675(A064413(n)).
 * https://oeis.org/A304736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of b(n) =
 *            A048675(A064413(n)); computed by generating the EKG sequence
 *            with a sieve, then taking exact A048675 values as canonical
 *            binary expansions and numbering distinct values by first
 *            occurrence
 * Range:     u8 n <= 434, u16 n <= 65539, u32 n <= 65539, u64 n <= 65539,
 *            u128 n <= 65539, big any n
 * Validated: standard, pass, 114 terms through n = 7682 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *g_rgs = NULL;
static size_t g_have = 0;

static void build_rgs(size_t N)
{
    size_t L = 8 * N + 100;
    uint32_t *spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *pidx = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc(L + 2, 1);
    uint32_t *ptr = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *e = (uint32_t *)calloc(N + 2, sizeof(uint32_t));
    uint32_t *rgs = (uint32_t *)calloc(N + 2, sizeof(uint32_t));
    size_t i, j, k;
    uint32_t c = 0;

    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            pidx[i] = ++c;
            for (j = i; j <= L; j += i)
                if (!spf[j]) spf[j] = (uint32_t)i;
        }
    }

    e[1] = 1; used[1] = 1;
    if (N >= 2) { e[2] = 2; used[2] = 1; }
    for (k = 3; k <= N; k++) {
        uint32_t x = e[k - 1];
        uint32_t best = 0xFFFFFFFFu;
        while (x > 1) {
            uint32_t p = spf[x];
            uint32_t cand = ptr[p] ? ptr[p] : p;
            while (x % p == 0) x /= p;
            while (used[cand]) cand += p;
            ptr[p] = cand;
            if (cand < best) best = cand;
        }
        used[best] = 1;
        e[k] = best;
    }

    {
        int *cnt = (int *)calloc((size_t)c + 64, sizeof(int));
        size_t pool_cap = 1024, pool_len = 0;
        int *pool = (int *)malloc(pool_cap * sizeof(int));
        size_t *off = (size_t *)calloc(N + 2, sizeof(size_t));
        uint32_t *len = (uint32_t *)calloc(N + 2, sizeof(uint32_t));
        uint64_t *hash = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
        uint32_t *distinct = (uint32_t *)calloc(N + 2, sizeof(uint32_t));
        uint32_t nd = 0;
        size_t top = N < 2 ? N : N;

        for (k = 1; k <= top; k++) {
            uint32_t x = e[k];
            size_t hi = 0, b;
            uint64_t h = 1469598103934665603ULL;
            while (x > 1) {
                uint32_t p = spf[x];
                int ex = 0;
                while (x % p == 0) { x /= p; ex++; }
                cnt[pidx[p] - 1] += ex;
                if ((size_t)pidx[p] > hi) hi = pidx[p];
            }
            off[k] = pool_len;
            len[k] = 0;
            for (b = 0; b < hi + 8; b++) {
                cnt[b + 1] += cnt[b] >> 1;
                cnt[b] &= 1;
                if (cnt[b]) {
                    if (pool_len >= pool_cap) {
                        pool_cap *= 2;
                        pool = (int *)realloc(pool, pool_cap * sizeof(int));
                    }
                    pool[pool_len++] = (int)b;
                    len[k]++;
                    h = (h ^ (uint64_t)(b + 1)) * 1099511628211ULL;
                    cnt[b] = 0;
                }
            }
            cnt[hi + 8] = 0;
            hash[k] = h;

            {
                uint32_t d, found = 0;
                for (d = 0; d < nd; d++) {
                    size_t q = distinct[d];
                    if (hash[q] == h && len[q] == len[k] &&
                        memcmp(pool + off[q], pool + off[k], len[k] * sizeof(int)) == 0) {
                        found = d + 1;
                        break;
                    }
                }
                if (!found) {
                    distinct[nd++] = (uint32_t)k;
                    found = nd;
                }
                rgs[k] = found;
            }
        }
        free(cnt); free(pool); free(off); free(len); free(hash); free(distinct);
    }

    free(spf); free(pidx); free(used); free(ptr); free(e);
    free(g_rgs);
    g_rgs = rgs;
    g_have = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if ((size_t)n > g_have) {
        size_t N = (size_t)n;
        if (N < 2 * g_have) N = 2 * g_have;
        if (N < 1000) N = 1000;
        build_rgs(N);
    }
    return (seq_t)g_rgs[(size_t)n];
}
