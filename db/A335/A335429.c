/*
 * A335429  Partial sums of A329697.
 * https://oeis.org/A335429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A329697(k), where A329697(k) = 0 if k is a
 *            power of 2, else 1 + A329697(k - k/lpf(k)), lpf = largest prime
 *            factor (table DP + sieve)
 * Range:     u8 n <= 104, u16 n <= 10691, u32 n <= 14042, u64 n <= 14042,
 *            u128 n <= 14042, big any n
 * Validated: standard, pass, 92 terms through n = 8522 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *g_f = 0;
static uint64_t *g_ps = 0;
static uint64_t g_cap = 0;

static void A335429_build(uint64_t N)
{
    uint64_t p, m, k;
    uint32_t *lpf;
    if (g_f) { free(g_f); free(g_ps); }
    g_f = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    g_ps = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    lpf = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    for (p = 2; p <= N; p++) {
        if (lpf[p] == 0) {
            for (m = p; m <= N; m += p) lpf[m] = (uint32_t)p;
        }
    }
    g_f[0] = 0;
    g_ps[0] = 0;
    for (k = 1; k <= N; k++) {
        if ((k & (k - 1)) == 0) g_f[k] = 0;
        else g_f[k] = 1 + g_f[k - k / lpf[k]];
        g_ps[k] = g_ps[k - 1] + g_f[k];
    }
    free(lpf);
    g_cap = N;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N > g_cap) {
        uint64_t c = g_cap * 2;
        if (c < 1024) c = 1024;
        if (c < N) c = N;
        A335429_build(c);
    }
    return (seq_t)g_ps[N];
}
