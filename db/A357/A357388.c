/*
 * A357388  Numbers k such that A071626(k) < A071626(k+1).
 * https://oeis.org/A357388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A071626(k) < A071626(k+1); A071626(k) =
 *            number of distinct exponents in k! (computed by incremental
 *            sieve, doubling the limit)
 * Range:     u8 n <= 22, u16 n <= 2343, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9583 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *A357388_terms = NULL;
static size_t A357388_nterms = 0;

static void A357388_build(uint32_t L)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 3, sizeof(uint32_t));
    uint32_t *e   = (uint32_t *)calloc((size_t)L + 3, sizeof(uint32_t));
    uint32_t *cnt = (uint32_t *)calloc((size_t)L + 3, sizeof(uint32_t));
    uint32_t *D   = (uint32_t *)calloc((size_t)L + 3, sizeof(uint32_t));
    if (!spf || !e || !cnt || !D) exit(EXIT_FAILURE);
    uint32_t M = L + 1;
    for (uint32_t i = 2; i <= M; ++i) {
        if (spf[i] == 0) {
            for (uint32_t j = i; j <= M; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    uint32_t distinct = 0;
    D[1] = 0;
    for (uint32_t k = 2; k <= M; ++k) {
        uint32_t x = k;
        while (x > 1) {
            uint32_t p = spf[x];
            uint32_t m = 0;
            while (x % p == 0) { x /= p; ++m; }
            uint32_t old = e[p];
            if (old > 0) {
                if (--cnt[old] == 0) --distinct;
            }
            uint32_t nw = old + m;
            if (cnt[nw]++ == 0) ++distinct;
            e[p] = nw;
        }
        D[k] = distinct;
    }
    free(A357388_terms);
    A357388_terms = (uint32_t *)malloc(((size_t)L + 1) * sizeof(uint32_t));
    if (!A357388_terms) exit(EXIT_FAILURE);
    size_t c = 0;
    for (uint32_t k = 1; k <= L; ++k)
        if (D[k] < D[k + 1]) A357388_terms[c++] = k;
    A357388_nterms = c;
    free(spf); free(e); free(cnt); free(D);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint32_t L = 4096;
    if (A357388_nterms == 0) A357388_build(L);
    while ((size_t)n > A357388_nterms) {
        /* limit doubles; derive current limit from last built size */
        static uint32_t cur = 4096;
        cur *= 2;
        A357388_build(cur);
    }
    return (seq_t)A357388_terms[(size_t)n - 1];
}
