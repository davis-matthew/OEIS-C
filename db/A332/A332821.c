/*
 * A332821  One part of a 3-way classification of the positive integers.
 *          Numbers n for which A048675(n) == 1 (mod 3).
 * https://oeis.org/A332821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A048675(k) == 1 (mod 3); A048675(k) mod 3 =
 *            sum over p^e||k of e*(+1 if PrimePi(p) odd else -1) mod 3;
 *            computed by a linear sieve, doubling the limit as needed
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static unsigned char *g_cls = NULL;
static uint64_t g_lim = 0;

static void build_cls(uint64_t N)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    uint32_t *pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)N / 2 + 2));
    unsigned char *g = (unsigned char *)calloc((size_t)N + 1, 1);
    size_t pc = 0;
    if (!spf || !pr || !g) { free(spf); free(pr); free(g); return; }
    for (uint64_t i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            pr[pc++] = (uint32_t)i;
            g[i] = (pc & 1) ? 1 : 2;
        } else {
            g[i] = (unsigned char)((g[i / spf[i]] + g[spf[i]]) % 3);
        }
        for (size_t j = 0; j < pc; j++) {
            uint64_t p = pr[j];
            if (p > spf[i] || i * p > N) break;
            spf[i * p] = (uint32_t)p;
        }
    }
    free(spf);
    free(pr);
    free(g_cls);
    g_cls = g;
    g_lim = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (g_lim == 0) build_cls(1024);
    for (;;) {
        uint64_t cnt = 0;
        for (uint64_t k = 1; k <= g_lim; k++) {
            if (g_cls[k] == 1) {
                if (++cnt == (uint64_t)n) return (seq_t)k;
            }
        }
        uint64_t old = g_lim;
        build_cls(g_lim * 2);
        if (g_lim == old) return 0;
    }
}
