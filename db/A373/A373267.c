/*
 * A373267  Numbers k for which A276085(k) is of the form 8m+4, where A276085
 *          is the primorial base log-function.
 * https://oeis.org/A373267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k-th number with A276085(k) == 4 (mod 8); A276085 is
 *            fully additive with A276085(p) = product of primes < p; computed
 *            by sieve with doubling limit
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9880 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *g_terms = NULL;
static size_t g_cnt = 0;

static void build_terms(uint64_t limit)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    uint8_t *pm = (uint8_t *)calloc((size_t)limit + 1, 1);
    uint8_t *f = (uint8_t *)calloc((size_t)limit + 1, 1);
    uint32_t *terms = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)limit + 1));
    size_t cnt = 0;
    uint32_t prod = 1; /* product of primes < current, mod 8 */
    for (uint64_t i = 2; i <= limit; i++) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            pm[i] = (uint8_t)prod;
            prod = (prod * (uint32_t)(i & 7)) & 7;
        }
    }
    for (uint64_t k = 2; k <= limit; k++) {
        uint32_t p = spf[k];
        f[k] = (uint8_t)((f[k / p] + pm[p]) & 7);
    }
    for (uint64_t k = 1; k <= limit; k++)
        if (f[k] == 4) terms[cnt++] = (uint32_t)k;
    free(spf);
    free(pm);
    free(f);
    free(g_terms);
    g_terms = terms;
    g_cnt = cnt;
}

seq_t a(idx_t n)
{
    static uint64_t limit = 0;
    if (limit == 0) {
        limit = 4096;
        build_terms(limit);
    }
    while ((uint64_t)n > g_cnt) {
        limit *= 2;
        build_terms(limit);
    }
    return (seq_t)g_terms[n - 1];
}
