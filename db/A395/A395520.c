/*
 * A395520  First differences of A036349.
 * https://oeis.org/A395520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A036349(n+1) - A036349(n), where A036349 lists the k with
 *            sopfr(k) even (equivalently an even number of odd prime factors
 *            counted with multiplicity); computed by a sieve that doubles its
 *            limit as needed
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Validated: partial, pass, 96 terms through n = 96 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *g_terms = NULL;
static uint64_t g_count = 0;

static void A395520_build(uint64_t limit)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    uint8_t *par = (uint8_t *)calloc((size_t)limit + 1, 1);
    uint32_t *terms = (uint32_t *)malloc(((size_t)limit + 1) * sizeof(uint32_t));
    if (!spf || !par || !terms) abort();
    uint64_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    par[1] = 0;
    terms[cnt++] = 1;
    for (uint64_t i = 2; i <= limit; ++i) {
        uint32_t p = spf[i];
        par[i] = (uint8_t)(par[i / p] ^ (p & 1u));
        if (par[i] == 0) terms[cnt++] = (uint32_t)i;
    }
    free(spf);
    free(par);
    if (g_terms) free(g_terms);
    g_terms = terms;
    g_count = cnt;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (!g_terms) A395520_build(1 << 12);
    while (g_count < (uint64_t)n + 1) {
        uint64_t lim = (g_count ? g_terms[g_count - 1] : 4096);
        A395520_build(lim * 2 + 16);
    }
    return (seq_t)(g_terms[n] - g_terms[n - 1]);
}
