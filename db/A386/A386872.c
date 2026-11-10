/*
 * A386872  Smallest k for which A385662(k) = n, or -1 if no such k exists.
 * https://oeis.org/A386872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = smallest k with A385662(k) = n, else -1; found by search:
 *            closed form A385662(k)=tau(k/2) when 4|k, then scan of k with
 *            spf sieve
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static const unsigned SMALLEST_CAP = 1u << 22;
static uint32_t *g_spf = NULL;

static void smallest_init(void)
{
    if (g_spf) return;
    g_spf = (uint32_t *)malloc((SMALLEST_CAP + 1) * sizeof(uint32_t));
    for (uint32_t i = 0; i <= SMALLEST_CAP; i++) g_spf[i] = 0;
    for (uint32_t i = 2; i <= SMALLEST_CAP; i++) {
        if (g_spf[i] == 0) {
            for (uint32_t j = i; j <= SMALLEST_CAP; j += i)
                if (g_spf[j] == 0) g_spf[j] = i;
        }
    }
}

static uint64_t smallest_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    unsigned __int128 r = 1 % m, x = b % m;
    while (e) {
        if (e & 1) r = (r * x) % m;
        x = (x * x) % m;
        e >>= 1;
    }
    return (uint64_t)r;
}

/* for odd m: f = #{d|m : m | d^d}, tau = number of divisors */
static void smallest_odd_info(uint32_t m, uint64_t *f, uint64_t *tau)
{
    uint64_t divs[4096];
    size_t cnt = 1;
    divs[0] = 1;
    uint64_t rad = 1;
    uint32_t t = m;
    while (t > 1) {
        uint32_t p = g_spf[t];
        rad *= p;
        size_t cur = cnt;
        uint64_t step = 1;
        while (t % p == 0) {
            t /= p;
            step *= p;
            for (size_t j = 0; j < cur; j++) divs[cnt++] = divs[j] * step;
        }
    }
    *tau = cnt;
    if (m == 1) { *f = 1; return; }
    uint64_t c = 0;
    for (size_t i = 0; i < cnt; i++) {
        uint64_t d = divs[i];
        if (d % rad != 0) continue;
        if (smallest_powmod(d, d, m) == 0) c++;
    }
    *f = c;
}

static const unsigned SMALLEST_PR[30] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,
    53,59,61,67,71,73,79,83,89,97,101,103,107,109,113};

static bool g_have;
static seq_t g_best;

static void smallest_dfs(unsigned idx, idx_t rem, idx_t maxe, seq_t cur)
{
    if (rem == 1) {
        if (!g_have || cur < g_best) { g_best = cur; g_have = true; }
        return;
    }
    if (idx >= 30) return;
    seq_t p = (seq_t)SMALLEST_PR[idx];
    for (idx_t t = 2; t <= rem; t++) {
        if (rem % t != 0 || t - 1 > maxe) continue;
        seq_t v = cur;
        bool ok = true;
        for (idx_t e = 0; e + 1 < t; e++) {
            seq_t nv = v * p;
            if (nv / p != v) { ok = false; break; }
            v = nv;
            if (g_have && v >= g_best) { ok = false; break; }
        }
        if (!ok) continue;
        smallest_dfs(idx + 1, rem / t, t - 1, v);
    }
}

seq_t a(idx_t n)
{
    smallest_init();
    g_have = false;
    g_best = 0;
    bool haveA = false;
    seq_t bestA = 0;
    if (n >= 2) {
        smallest_dfs(0, n, (idx_t)200, (seq_t)1);
        if (g_have) {
            seq_t k = g_best * 2;
            if (k / 2 == g_best) { haveA = true; bestA = k; }
        }
    }
    uint32_t L = SMALLEST_CAP;
    if (haveA && bestA <= (seq_t)SMALLEST_CAP) L = (uint32_t)bestA;
    for (uint32_t k = 1; k <= L; k++) {
        if (k % 4 == 0) {
            if (haveA && (seq_t)k == bestA) return bestA;
            continue;
        }
        uint64_t f, tau, val;
        if (k & 1) {
            smallest_odd_info(k, &f, &tau);
            val = f;
        } else {
            smallest_odd_info(k / 2, &f, &tau);
            val = f + tau;
        }
        if ((seq_t)val == (seq_t)n) return (seq_t)k;
    }
    if (haveA) return bestA;
    return (seq_t)0 - (seq_t)1;
}
