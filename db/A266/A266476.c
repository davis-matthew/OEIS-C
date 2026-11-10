/*
 * A266476  Ordinal transform of A266475.
 * https://oeis.org/A266476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of m <= n with A266475(m) = A266475(n); A266475(m)
 *            = sum of prime indices (with multiplicity) +
 *            Omega(m)*(Omega(m)-1)/2, computed by a smallest-prime-factor
 *            sieve and counting
 * Range:     u8 n <= 13803, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 116 terms through n = 8639 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A266476_res = NULL;
static uint64_t A266476_lim = 0;

static void A266476_build(uint64_t L)
{
    uint32_t *spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *pidx = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint64_t *S = (uint64_t *)calloc(L + 1, sizeof(uint64_t));
    uint32_t *O = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *cnt = (uint32_t *)calloc(L + 2200, sizeof(uint32_t));
    uint32_t *res = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint64_t m, j, k = 0;

    for (m = 2; m <= L; m++) {
        if (spf[m] == 0) {
            for (j = m; j <= L; j += m)
                if (spf[j] == 0) spf[j] = (uint32_t)m;
        }
    }
    res[1] = 1;
    cnt[0] = 1;
    for (m = 2; m <= L; m++) {
        uint64_t p = spf[m];
        if (p == m) {
            pidx[m] = (uint32_t)(++k);
            S[m] = pidx[m];
            O[m] = 1;
        } else {
            S[m] = S[m / p] + S[p];
            O[m] = O[m / p] + 1;
        }
        uint64_t o = O[m];
        uint64_t v = S[m] + o * (o - 1) / 2;
        res[m] = ++cnt[v];
    }
    free(spf); free(pidx); free(S); free(O); free(cnt);
    free(A266476_res);
    A266476_res = res;
    A266476_lim = L;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A266476_res == NULL || n > A266476_lim) {
        uint64_t L = (uint64_t)n;
        if (L < 2 * A266476_lim) L = 2 * A266476_lim;
        if (L < 1024) L = 1024;
        A266476_build(L);
    }
    return (seq_t)A266476_res[n];
}
