/*
 * A332952  Squarefree semiprimes which never occur in A245486.
 * https://oeis.org/A332952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th squarefree semiprime p*q (p<q) with no k such that
 *            A006530(k)*A006530(k+1) = p*q (A245486); computed by sieving gpf
 *            up to 2^23 and marking products
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A332952_L    (1u << 23)
#define A332952_BMAX 65536u

static uint32_t *A332952_terms = NULL;
static size_t A332952_cnt = 0;

static void A332952_build(void)
{
    uint32_t *g = (uint32_t *)calloc((size_t)A332952_L + 2, sizeof(uint32_t));
    unsigned char *mark = (unsigned char *)calloc(A332952_BMAX + 1, 1);
    if (!g || !mark) abort();
    g[1] = 1;
    for (uint32_t p = 2; p <= A332952_L + 1; ++p) {
        if (g[p] == 0) {
            for (uint64_t m = p; m <= (uint64_t)A332952_L + 1; m += p)
                g[m] = p;
        }
    }
    for (uint32_t k = 1; k <= A332952_L; ++k) {
        uint64_t prod = (uint64_t)g[k] * g[k + 1];
        if (prod <= A332952_BMAX) mark[prod] = 1;
    }
    free(g);

    uint32_t *terms = (uint32_t *)malloc(sizeof(uint32_t) * (A332952_BMAX + 1));
    if (!terms) abort();
    size_t cnt = 0;
    for (uint32_t N = 6; N <= A332952_BMAX; ++N) {
        if (mark[N]) continue;
        /* find smallest prime factor */
        uint32_t p = 0;
        for (uint32_t d = 2; (uint64_t)d * d <= N; ++d)
            if (N % d == 0) { p = d; break; }
        if (!p) continue;
        uint32_t q = N / p;
        if (q <= p) continue;
        int prime = 1;
        for (uint32_t d = 2; (uint64_t)d * d <= q; ++d)
            if (q % d == 0) { prime = 0; break; }
        if (!prime) continue;
        terms[cnt++] = N;
    }
    free(mark);
    A332952_terms = terms;
    A332952_cnt = cnt;
}

seq_t a(idx_t n)
{
    if (A332952_terms == NULL) A332952_build();
    if (n < 1 || (size_t)n > A332952_cnt) return (seq_t)0;
    return (seq_t)A332952_terms[(size_t)n - 1];
}
