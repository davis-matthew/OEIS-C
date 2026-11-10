/*
 * A270202  Permutation of natural numbers: a(1) = 1, a(2n) = A270189(1+a(n)),
 *          a(2n+1) = A270190(a(n)).
 * https://oeis.org/A270202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(1) = 1, a(2n) = A270189(1+a(n)), a(2n+1) = A270190(a(n));
 *            A270189/A270190 are the indices k where prime(k+1)-prime(k) is
 *            non-multiple/multiple of 3 (computed by sieve)
 * Range:     u8 n <= 30, u16 n <= 2046, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 96 terms through n = 8153 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *g_nm = NULL;   /* indices k with gap not multiple of 3 */
static uint32_t *g_mu = NULL;   /* indices k with gap multiple of 3 */
static uint64_t g_nm_cnt = 0, g_mu_cnt = 0;
static uint64_t g_lim = 0;

static void rebuild_gap_lists(uint64_t lim) {
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    uint64_t i, j;
    uint64_t pk = 0, prev = 0;
    if (!comp) exit(1);
    for (i = 2; i * i <= lim; i++)
        if (!comp[i])
            for (j = i * i; j <= lim; j += i) comp[j] = 1;
    free(g_nm); free(g_mu);
    g_nm = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(lim / 2 + 2));
    g_mu = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(lim / 4 + 2));
    if (!g_nm || !g_mu) exit(1);
    g_nm_cnt = 0; g_mu_cnt = 0;
    for (i = 2; i <= lim; i++) {
        if (comp[i]) continue;
        pk++;
        if (prev) {
            /* gap between prime(pk-1) and prime(pk), index pk-1 */
            if ((i - prev) % 3 == 0) g_mu[g_mu_cnt++] = (uint32_t)(pk - 1);
            else g_nm[g_nm_cnt++] = (uint32_t)(pk - 1);
        }
        prev = i;
    }
    free(comp);
    g_lim = lim;
}

static void ensure_lists(uint64_t need_nm, uint64_t need_mu) {
    uint64_t lim = g_lim ? g_lim : (1u << 16);
    while (g_lim == 0 || g_nm_cnt < need_nm || g_mu_cnt < need_mu) {
        if (g_lim != 0) lim = g_lim * 2;
        rebuild_gap_lists(lim);
    }
}

seq_t a(idx_t n) {
    int top = 63;
    uint64_t v = 1;
    int b;
    if (n <= 1) return (seq_t)1;
    while (!((n >> top) & 1)) top--;
    for (b = top - 1; b >= 0; b--) {
        if ((n >> b) & 1) {
            ensure_lists(0, v);
            v = g_mu[v - 1];
        } else {
            ensure_lists(v, 0);
            v = g_nm[v];   /* A270189(1+v) -> 0-based index v */
        }
    }
    return (seq_t)v;
}
