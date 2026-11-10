/*
 * A358778  Positions of positive terms in A358777, which is the Dirichlet
 *          inverse of A353557.
 * https://oeis.org/A358778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A358777(k) > 0, where A358777 is the
 *            Dirichlet inverse of A353557 (f(m)=1 iff m odd and bigomega(m)
 *            even); computed by a sieve with doubling limit
 * Range:     u8 n <= 4, u16 n <= 3738, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 71 terms through n = 8722 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static idx_t *pos_list = NULL;
static size_t pos_len = 0;
static size_t pos_cap = 0;
static size_t pos_limit = 0;

static void pos_compute(size_t L)
{
    uint32_t *spf = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
    unsigned char *good = (unsigned char *)malloc(L + 1);
    unsigned char *par = (unsigned char *)malloc(L + 1);
    int64_t *S = (int64_t *)malloc((L + 1) * sizeof(int64_t));
    if (!spf || !good || !par || !S) abort();
    memset(spf, 0, (L + 1) * sizeof(uint32_t));
    memset(S, 0, (L + 1) * sizeof(int64_t));
    for (size_t i = 2; i <= L; ++i) {
        if (spf[i] == 0) {
            for (size_t j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    par[0] = 0;
    if (L >= 1) par[1] = 0;
    for (size_t m = 2; m <= L; ++m) par[m] = par[m / spf[m]] ^ 1;
    for (size_t m = 0; m <= L; ++m)
        good[m] = (m & 1) && par[m] == 0;

    pos_len = 0;
    for (size_t d = 1; d <= L; d += 2) {
        int64_t v;
        if (d == 1) v = 1; else v = -S[d];
        if (v == 0) continue;
        if (v > 0) {
            if (pos_len == pos_cap) {
                pos_cap = pos_cap ? pos_cap * 2 : 1024;
                idx_t *t = (idx_t *)realloc(pos_list, pos_cap * sizeof(idx_t));
                if (!t) abort();
                pos_list = t;
            }
            pos_list[pos_len++] = (idx_t)d;
        }
        size_t kmax = L / d;
        for (size_t k = 3; k <= kmax; k += 2)
            if (good[k]) S[d * k] += v;
    }
    pos_limit = L;
    free(spf); free(good); free(par); free(S);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (pos_limit == 0) pos_compute(1u << 16);
    while (pos_len < (size_t)n) pos_compute(pos_limit * 2);
    return (seq_t)pos_list[n - 1];
}
