/*
 * A381926  Smallest divisor d of A381925(n) for which sigma(d) =
 *          tau(A381925(n)).
 * https://oeis.org/A381926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least divisor d of k with sigma(d) = tau(k), where k =
 *            A381925(n); computed by sieve and search
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9710 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *g_tau = NULL;
static size_t g_lim = 0;
static uint64_t *g_res = NULL;
static size_t g_len = 0, g_cap = 0;
static uint64_t g_k = 0;

static void build_tau(size_t limit)
{
    size_t i, j;
    free(g_tau);
    g_tau = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!g_tau) abort();
    for (i = 1; i <= limit; ++i)
        for (j = i; j <= limit; j += i) ++g_tau[j];
    g_lim = limit;
}

static uint64_t sig_small(uint64_t d)
{
    uint64_t s = 1, p;
    uint64_t m = d;
    for (p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t term = 1, pw = 1;
            while (m % p == 0) { m /= p; pw *= p; term += pw; }
            s *= term;
        }
    }
    if (m > 1) s *= (m + 1);
    return s;
}

seq_t a(idx_t n)
{
    if (g_lim == 0) build_tau(1024);
    while (g_len < (size_t)n) {
        uint64_t k = g_k + 1;
        uint64_t t, d;
        if (k > g_lim) build_tau(g_lim * 2);
        t = g_tau[k];
        for (d = 1; d <= t && d <= k; ++d) {
            if (k % d == 0 && sig_small(d) == t) {
                if (g_len == g_cap) {
                    size_t nc = g_cap ? g_cap * 2 : 64;
                    uint64_t *tmp = (uint64_t *)realloc(g_res, nc * sizeof(uint64_t));
                    if (!tmp) abort();
                    g_res = tmp;
                    g_cap = nc;
                }
                g_res[g_len++] = d;
                break;
            }
        }
        g_k = k;
    }
    return (seq_t)g_res[(size_t)n - 1];
}
