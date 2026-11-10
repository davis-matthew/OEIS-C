/*
 * A291634  Number of primitive sequences n = b_1 < b_2 < ... < b_t =
 *          A006255(n) such that b_1*b_2*...*b_t is a perfect square.
 * https://oeis.org/A291634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search: a(n) = number of sets S in (n, A006255(n)) such that
 *            v(n) + v(m) + sum_{s in S} v(s) = 0 over GF(2) (m = A006255(n),
 *            v(k) the exponent-parity vector of k) and {v(n)} u {v(s): s in
 *            S} is linearly independent, which makes the whole sequence a
 *            minimal square product; a(n) = 1 if n is a squar...
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static int A006255_reduce_vec(uint64_t *v, uint64_t **basis, int W) {
    int w = W - 1;
    while (w >= 0) {
        if (!v[w]) { w--; continue; }
        int b = 63 - __builtin_clzll(v[w]);
        int h = w * 64 + b;
        if (!basis[h]) return h;
        {
            uint64_t *bv = basis[h];
            int i;
            for (i = 0; i <= w; i++) v[i] ^= bv[i];
        }
    }
    return -1;
}

static void A006255_set_vec(uint64_t x, const uint32_t *spf, const int *pidx, uint64_t *v, int W) {
    memset(v, 0, (size_t)W * sizeof(uint64_t));
    while (x > 1) {
        uint32_t p = spf[x];
        int e = 0;
        while (x % p == 0) { x /= p; e++; }
        if (e & 1) {
            int k = pidx[p];
            v[k >> 6] ^= (uint64_t)1 << (k & 63);
        }
    }
}

static uint64_t A006255_a(uint64_t n) {
    uint64_t L, i, j, m, result = 0;
    uint32_t *spf;
    int *pidx;
    int P = 0, W, used = 0;
    uint64_t *vn, *vm, *t, *pool;
    uint64_t **basis;

    if (n <= 1) return 1;
    L = 4 * n + 16;
    spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    pidx = (int *)calloc((size_t)(L + 1), sizeof(int));
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            pidx[i] = P++;
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    W = (P + 63) / 64;
    vn = (uint64_t *)malloc((size_t)W * sizeof(uint64_t));
    vm = (uint64_t *)malloc((size_t)W * sizeof(uint64_t));
    t = (uint64_t *)malloc((size_t)W * sizeof(uint64_t));
    pool = (uint64_t *)malloc((size_t)P * (size_t)W * sizeof(uint64_t));
    basis = (uint64_t **)calloc((size_t)P, sizeof(uint64_t *));

    A006255_set_vec(n, spf, pidx, vn, W);
    {
        bool zero = true;
        int k;
        for (k = 0; k < W; k++) if (vn[k]) { zero = false; break; }
        if (zero) result = n;
    }
    for (m = n + 1; result == 0 && m <= L; m++) {
        int k, h;
        A006255_set_vec(m, spf, pidx, vm, W);
        for (k = 0; k < W; k++) t[k] = vn[k] ^ vm[k];
        if (A006255_reduce_vec(t, basis, W) < 0) { result = m; break; }
        h = A006255_reduce_vec(vm, basis, W);
        if (h >= 0) {
            uint64_t *slot = pool + (size_t)used * (size_t)W;
            memcpy(slot, vm, (size_t)W * sizeof(uint64_t));
            basis[h] = slot;
            used++;
        }
    }
    free(spf); free(pidx); free(vn); free(vm); free(t); free(pool); free(basis);
    return result;
}

/* search state */
static int g_W, g_P;
static uint64_t g_n, g_m;
static uint64_t *g_vec;      /* vector of n+d at g_vec + d*W */
static uint64_t *g_sb;       /* suffix basis: P slots * W */
static uint64_t *g_sbidx;    /* index stamp per slot; 0 = empty */
static uint64_t *g_ib;       /* independence basis: P slots * W */
static unsigned char *g_ibu; /* slot used */
static uint64_t *g_tmp;      /* scratch pool per depth */
static uint64_t g_count;

/* Insert vector with index pos; basis prefers LARGER indices so that the
   slots with index >= k span exactly span{v_k, ..., v_{m-1}}. */
static void sb_insert(const uint64_t *src, uint64_t pos) {
    int W = g_W;
    uint64_t *x = (uint64_t *)malloc((size_t)W * sizeof(uint64_t));
    memcpy(x, src, (size_t)W * sizeof(uint64_t));
    for (;;) {
        int w = W - 1, h;
        while (w >= 0 && !x[w]) w--;
        if (w < 0) break;
        h = w * 64 + 63 - __builtin_clzll(x[w]);
        if (!g_sbidx[h]) {
            memcpy(g_sb + (size_t)h * W, x, (size_t)W * sizeof(uint64_t));
            g_sbidx[h] = pos;
            break;
        }
        if (g_sbidx[h] < pos) {
            int i;
            uint64_t tp = g_sbidx[h];
            for (i = 0; i < W; i++) {
                uint64_t tt = g_sb[(size_t)h * W + i];
                g_sb[(size_t)h * W + i] = x[i];
                x[i] = tt;
            }
            g_sbidx[h] = pos;
            pos = tp;
        }
        {
            int i;
            for (i = 0; i < W; i++) x[i] ^= g_sb[(size_t)h * W + i];
        }
    }
    free(x);
}

/* is r in span of vectors with index in [k, m) ? */
static bool sb_query(const uint64_t *r, uint64_t k, uint64_t *x) {
    int W = g_W;
    memcpy(x, r, (size_t)W * sizeof(uint64_t));
    for (;;) {
        int w = W - 1, h, i;
        while (w >= 0 && !x[w]) w--;
        if (w < 0) return true;
        h = w * 64 + 63 - __builtin_clzll(x[w]);
        if (!g_sbidx[h] || g_sbidx[h] < k) return false;
        for (i = 0; i < W; i++) x[i] ^= g_sb[(size_t)h * W + i];
    }
}

static bool ib_reduce(uint64_t *v, int *hout) {
    int W = g_W;
    for (;;) {
        int w = W - 1, h, i;
        while (w >= 0 && !v[w]) w--;
        if (w < 0) return false;
        h = w * 64 + 63 - __builtin_clzll(v[w]);
        if (!g_ibu[h]) { *hout = h; return true; }
        for (i = 0; i < W; i++) v[i] ^= g_ib[(size_t)h * W + i];
    }
}

static void dfs(uint64_t k, const uint64_t *r, int depth) {
    int W = g_W, i;
    uint64_t j;
    bool zero = true;
    uint64_t *scratch = g_tmp + (size_t)depth * 3 * W;
    uint64_t *nr = scratch + W;
    uint64_t *cv = scratch + 2 * W;
    for (i = 0; i < W; i++) if (r[i]) { zero = false; break; }
    if (zero) { g_count++; return; }
    for (j = k; j < g_m; j++) {
        int h;
        if (!sb_query(r, j, scratch)) break;
        memcpy(cv, g_vec + (size_t)(j - g_n) * W, (size_t)W * sizeof(uint64_t));
        if (!ib_reduce(cv, &h)) continue;
        memcpy(g_ib + (size_t)h * W, cv, (size_t)W * sizeof(uint64_t));
        g_ibu[h] = 1;
        for (i = 0; i < W; i++) nr[i] = r[i] ^ g_vec[(size_t)(j - g_n) * W + i];
        dfs(j + 1, nr, depth + 1);
        g_ibu[h] = 0;
    }
}

seq_t a(idx_t n_idx) {
    uint64_t n = (uint64_t)n_idx, m, i, j, L;
    uint32_t *spf;
    int *pidx;
    int P = 0, W, h;
    uint64_t *r, *tmp0;
    uint64_t result;

    if (n <= 1) return (seq_t)1;
    m = A006255_a(n);
    if (m == n) return (seq_t)1;
    L = m;
    spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    pidx = (int *)calloc((size_t)(L + 1), sizeof(int));
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            pidx[i] = P++;
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    W = (P + 63) / 64;
    g_W = W; g_P = P; g_n = n; g_m = m; g_count = 0;
    g_vec = (uint64_t *)malloc((size_t)(m - n + 1) * W * sizeof(uint64_t));
    for (i = n; i <= m; i++)
        A006255_set_vec(i, spf, pidx, g_vec + (size_t)(i - n) * W, W);
    g_sb = (uint64_t *)calloc((size_t)P * W, sizeof(uint64_t));
    g_sbidx = (uint64_t *)calloc((size_t)P, sizeof(uint64_t));
    g_ib = (uint64_t *)calloc((size_t)P * W, sizeof(uint64_t));
    g_ibu = (unsigned char *)calloc((size_t)P, 1);
    g_tmp = (uint64_t *)calloc((size_t)(P + 3) * 3 * W, sizeof(uint64_t));
    r = (uint64_t *)malloc((size_t)W * sizeof(uint64_t));
    tmp0 = (uint64_t *)malloc((size_t)W * sizeof(uint64_t));

    for (i = m - 1; i > n; i--) sb_insert(g_vec + (size_t)(i - n) * W, i);

    /* r = v(n) ^ v(m); independence basis starts with v(n) */
    for (j = 0; j < (uint64_t)W; j++) r[j] = g_vec[j] ^ g_vec[(size_t)(m - n) * W + j];
    memcpy(tmp0, g_vec, (size_t)W * sizeof(uint64_t));
    if (ib_reduce(tmp0, &h)) {
        memcpy(g_ib + (size_t)h * W, tmp0, (size_t)W * sizeof(uint64_t));
        g_ibu[h] = 1;
    }
    dfs(n + 1, r, 0);
    result = g_count;

    free(spf); free(pidx); free(g_vec); free(g_sb); free(g_sbidx);
    free(g_ib); free(g_ibu); free(g_tmp); free(r); free(tmp0);
    return (seq_t)result;
}
