/*
 * A269045  Indices k such that A006255(k) != A070229(k); that is, the k-th
 *          term of R. L. Graham's sequence is not equal to k + lpf(k).
 * https://oeis.org/A269045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A006255(k) != A070229(k) = k + gpf(k)
 *            (a(1) of A070229 is 2); computed by a sieve plus a GF(2)
 *            elimination search, with a heuristic shortcut when gpf(k) > 311
 * Range:     u8 n <= 99, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 78 terms through n = 2197 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *g_spf = 0;
static int *g_idmap = 0;
static uint64_t g_L = 0;
static uint64_t *g_terms = 0;
static size_t g_cnt = 0, g_cap = 0;
static uint64_t g_checked = 0;
static int g_sidx[320];
static int g_sinit = 0;

static void build_sieve(uint64_t L) {
    uint64_t i, j;
    if (g_spf) free(g_spf);
    if (g_idmap) free(g_idmap);
    g_spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    g_idmap = (int *)calloc((size_t)(L + 1), sizeof(int));
    for (i = 2; i <= L; i++) {
        if (g_spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (g_spf[j] == 0) g_spf[j] = (uint32_t)i;
        }
    }
    g_L = L;
}

static int oddp(uint64_t x, uint32_t *out) {
    int c = 0;
    while (x > 1) {
        uint32_t p = g_spf[x];
        int e = 0;
        while (x % p == 0) { x /= p; e++; }
        if (e & 1) out[c++] = p;
    }
    return c;
}

static void tog(uint32_t *s, int *c, uint32_t p) {
    int i;
    for (i = 0; i < *c; i++) {
        if (s[i] == p) { s[i] = s[*c - 1]; (*c)--; return; }
    }
    s[(*c)++] = p;
}

static int gdiff_reduce(uint64_t *v, uint64_t **basis, int W) {
    int w = W - 1;
    while (w >= 0) {
        if (!v[w]) { w--; continue; }
        {
            int b = 63 - __builtin_clzll(v[w]);
            int h = w * 64 + b;
            int i;
            if (!basis[h]) return h;
            for (i = 0; i <= w; i++) v[i] ^= basis[h][i];
        }
    }
    return -1;
}

/* exact scan for the case where gpf(k) has even exponent (g small) */
static bool generic_differ(uint64_t k, uint64_t g) {
    uint32_t ps[16];
    uint64_t x, m;
    int D = 0, W, used = 0, c, i;
    uint32_t *touched;
    size_t tcnt = 0;
    uint64_t *vk, *vm, *t, *pool;
    uint64_t **basis;
    bool found = false;
    uint64_t result = 0;

    touched = (uint32_t *)malloc((size_t)(g + 2) * 16 * sizeof(uint32_t));
    for (x = k; x <= k + g; x++) {
        c = oddp(x, ps);
        for (i = 0; i < c; i++) {
            if (!g_idmap[ps[i]]) {
                g_idmap[ps[i]] = ++D;
                touched[tcnt++] = ps[i];
            }
        }
    }
    W = (D + 63) / 64;
    if (W < 1) W = 1;
    vk = (uint64_t *)calloc((size_t)W, sizeof(uint64_t));
    vm = (uint64_t *)calloc((size_t)W, sizeof(uint64_t));
    t = (uint64_t *)calloc((size_t)W, sizeof(uint64_t));
    pool = (uint64_t *)calloc((size_t)(g + 2) * (size_t)W, sizeof(uint64_t));
    basis = (uint64_t **)calloc((size_t)(D + 1), sizeof(uint64_t *));

    c = oddp(k, ps);
    for (i = 0; i < c; i++) { int id = g_idmap[ps[i]] - 1; vk[id >> 6] ^= (uint64_t)1 << (id & 63); }

    for (m = k + 1; m <= k + g; m++) {
        int h, j;
        memset(vm, 0, (size_t)W * sizeof(uint64_t));
        c = oddp(m, ps);
        for (i = 0; i < c; i++) { int id = g_idmap[ps[i]] - 1; vm[id >> 6] ^= (uint64_t)1 << (id & 63); }
        for (j = 0; j < W; j++) t[j] = vk[j] ^ vm[j];
        if (gdiff_reduce(t, basis, W) < 0) { found = true; result = m; break; }
        h = gdiff_reduce(vm, basis, W);
        if (h >= 0) {
            uint64_t *slot = pool + (size_t)used * (size_t)W;
            memcpy(slot, vm, (size_t)W * sizeof(uint64_t));
            basis[h] = slot;
            used++;
        }
    }

    for (i = 0; i < (int)tcnt; i++) g_idmap[touched[i]] = 0;
    free(touched); free(vk); free(vm); free(t); free(pool); free(basis);
    if (!found) return true;
    return result != k + g;
}

static bool is_differ(uint64_t k) {
    uint32_t pk[16], p2[16], xs[16], ts[200];
    uint64_t used[16];
    int nu = 0, c, c2, tc = 0, i, j;
    uint64_t g = 0, x;
    uint32_t r;

    if (k == 1) return true;
    c = oddp(k, pk);
    if (c == 0) return true;
    x = k;
    while (x > 1) {
        uint32_t p = g_spf[x];
        g = p;
        while (x % p == 0) x /= p;
    }
    r = pk[c - 1];
    if (r != g) {
        if (g > 311) return true;
        return generic_differ(k, g);
    }
    for (i = 0; i < c; i++) tog(ts, &tc, pk[i]);
    c2 = oddp(k + g, p2);
    for (i = 0; i < c2; i++) tog(ts, &tc, p2[i]);

    for (;;) {
        uint32_t big = 0;
        uint64_t y;
        int cx;
        bool hasr = false;
        for (i = 0; i < tc; i++) if (ts[i] > g) { big = ts[i]; break; }
        if (!big) break;
        y = (k / big + 1) * big;
        if (y >= k + g) return true;
        for (i = 0; i < nu; i++) if (used[i] == y) return true;
        if (nu >= 16) return true;
        used[nu++] = y;
        cx = oddp(y, xs);
        for (i = 0; i < cx; i++) if (xs[i] == big) hasr = true;
        if (!hasr) return true;
        for (i = 0; i < cx; i++) tog(ts, &tc, xs[i]);
        if (tc > 150) return true;
    }
    for (i = 0; i < tc; i++) if (ts[i] == g) return true;
    if (g > 311) return false;

    {
        uint64_t basis[64];
        uint64_t tm = 0, v;
        memset(basis, 0, sizeof(basis));
        for (i = 0; i < tc; i++) tm |= (uint64_t)1 << g_sidx[ts[i]];
        for (x = k + 1; x < k + g; x++) {
            int cx = oddp(x, xs);
            bool ok = true;
            v = 0;
            for (j = 0; j < cx; j++) {
                if (xs[j] > g) { ok = false; break; }
                v |= (uint64_t)1 << g_sidx[xs[j]];
            }
            if (!ok) continue;
            while (v) {
                int h = 63 - __builtin_clzll(v);
                if (!basis[h]) { basis[h] = v; break; }
                v ^= basis[h];
            }
        }
        while (tm) {
            int h = 63 - __builtin_clzll(tm);
            if (!basis[h]) return true;
            tm ^= basis[h];
        }
        return false;
    }
}

seq_t a(idx_t n_idx) {
    uint64_t n = (uint64_t)n_idx;
    if (!g_sinit) {
        int i, j, cnt = 0;
        memset(g_sidx, 0, sizeof(g_sidx));
        for (i = 2; i < 320; i++) {
            bool pr = true;
            for (j = 2; j * j <= i; j++) if (i % j == 0) { pr = false; break; }
            if (pr) { if (cnt < 64) g_sidx[i] = cnt; cnt++; }
        }
        g_sinit = 1;
    }
    while (g_cnt < n) {
        uint64_t newK = g_checked ? g_checked * 2 : 4 * n + 64;
        uint64_t k;
        build_sieve(2 * newK + 4);
        for (k = g_checked + 1; k <= newK; k++) {
            if (is_differ(k)) {
                if (g_cnt == g_cap) {
                    g_cap = g_cap ? g_cap * 2 : 1024;
                    g_terms = (uint64_t *)realloc(g_terms, g_cap * sizeof(uint64_t));
                }
                g_terms[g_cnt++] = k;
            }
        }
        g_checked = newK;
    }
    return (seq_t)g_terms[n - 1];
}
