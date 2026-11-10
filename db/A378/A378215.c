/*
 * A378215  Parity of A378213, where A378213 is the Dirichlet inverse of
 *          A140773.
 * https://oeis.org/A378215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A378213(n) mod 2, where A378213 is the Dirichlet inverse
 *            of A140773 (A140773(n) = Sum_{m|n} ceil(d(m)/2)); computed by
 *            sieve: g(1)=1, g(n) = Sum_{d|n, d>1} f(d)*g(n/d) mod 2
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 6911 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned char *g378215_tab = 0;
static uint64_t g378215_lim = 0;

static void g378215_build(uint64_t N)
{
    uint64_t i, j;
    uint32_t *dc = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    unsigned char *f = (unsigned char *)calloc(N + 1, 1);
    unsigned char *acc = (unsigned char *)calloc(N + 1, 1);
    for (i = 1; i <= N; i++)
        for (j = i; j <= N; j += i) dc[j]++;
    for (i = 1; i <= N; i++) {
        unsigned char c = (unsigned char)(((dc[i] + 1) / 2) & 1);
        for (j = i; j <= N; j += i) f[j] ^= c;
    }
    for (i = 1; i <= N; i++) {
        unsigned char gi = (i == 1) ? 1 : acc[i];
        acc[i] = gi;
        if (gi) {
            for (j = 2; i * j <= N; j++)
                if (f[j]) acc[i * j] ^= 1;
        }
    }
    free(dc);
    free(f);
    if (g378215_tab) free(g378215_tab);
    g378215_tab = acc;
    g378215_lim = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > g378215_lim) {
        uint64_t N = g378215_lim * 2;
        if (N < (uint64_t)n) N = (uint64_t)n;
        if (N < 1024) N = 1024;
        g378215_build(N);
    }
    return (seq_t)g378215_tab[n];
}
