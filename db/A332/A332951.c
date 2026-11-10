/*
 * A332951  Numbers m such that A245486(k) = m for some k.
 * https://oeis.org/A332951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest value of A245486(k)=gpf(k)*gpf(k+1), found
 *            by sieving gpf for k up to 2^24 and collecting the distinct
 *            products (<= 2^20) in increasing order
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *A332951_terms = NULL;
static size_t A332951_cnt = 0;
static int A332951_ready = 0;

static void A332951_build(void)
{
    const size_t L = (size_t)1 << 24;
    const uint64_t M = (uint64_t)1 << 20;
    uint32_t *g = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    unsigned char *mark = (unsigned char *)calloc((size_t)M + 1, 1);
    if (!g || !mark) abort();
    g[1] = 1;
    for (size_t p = 2; p <= L + 1; ++p) {
        if (g[p] == 0) {
            for (size_t m = p; m <= L + 1; m += p)
                g[m] = (uint32_t)p;
        }
    }
    size_t cnt = 0;
    for (size_t k = 1; k <= L; ++k) {
        uint64_t v = (uint64_t)g[k] * (uint64_t)g[k + 1];
        if (v <= M && !mark[v]) {
            mark[v] = 1;
            ++cnt;
        }
    }
    A332951_terms = (uint32_t *)malloc((cnt + 1) * sizeof(uint32_t));
    if (!A332951_terms) abort();
    size_t j = 0;
    for (uint64_t v = 1; v <= M; ++v)
        if (mark[v]) A332951_terms[j++] = (uint32_t)v;
    A332951_cnt = j;
    free(g);
    free(mark);
    A332951_ready = 1;
}

seq_t a(idx_t n)
{
    if (!A332951_ready) A332951_build();
    if (n < 1 || (size_t)n > A332951_cnt) return (seq_t)0;
    return (seq_t)A332951_terms[(size_t)(n - 1)];
}
