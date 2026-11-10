/*
 * A359598  Indices of terms with record absolute values in A358777.
 * https://oeis.org/A359598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = index of the n-th record of |A358777(k)|, where A358777
 *            is the Dirichlet inverse of A353557 (1 if k odd and bigomega(k)
 *            even, else 0). Method: A358777 depends only on the prime
 *            signature of odd k, and is 0 for even k. Records occur only at
 *            the smallest number of each signature, i.e. 3^e1*5...
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

#define A359598_MAXL 24

static seq_t   *A359598_mag = NULL;
static uint8_t *A359598_neg = NULL;
static uint8_t *A359598_ex  = NULL;   /* cap * MAXL */
static uint8_t *A359598_ln  = NULL;
static size_t   A359598_cnt = 0, A359598_cap = 0;
static int32_t *A359598_tab = NULL;
static size_t   A359598_tsz = 0;

static uint64_t A359598_hash(const uint8_t *e, int len)
{
    uint64_t h = 1469598103934665603ULL;
    for (int i = 0; i < len; ++i) { h ^= e[i]; h *= 1099511628211ULL; }
    h ^= (uint64_t)len * 0x9E3779B97F4A7C15ULL;
    return h;
}

static long A359598_find(const uint8_t *e, int len, size_t *slot)
{
    size_t mask = A359598_tsz - 1;
    size_t s = (size_t)(A359598_hash(e, len) & mask);
    while (A359598_tab[s] >= 0) {
        size_t k = (size_t)A359598_tab[s];
        if (A359598_ln[k] == len && memcmp(A359598_ex + k * A359598_MAXL, e, (size_t)len) == 0) {
            *slot = s;
            return (long)k;
        }
        s = (s + 1) & mask;
    }
    *slot = s;
    return -1;
}

static void A359598_rehash(void)
{
    size_t nsz = A359598_tsz ? A359598_tsz * 2 : (1u << 16);
    int32_t *nt = (int32_t *)malloc(nsz * sizeof(int32_t));
    if (!nt) exit(EXIT_FAILURE);
    for (size_t i = 0; i < nsz; ++i) nt[i] = -1;
    free(A359598_tab);
    A359598_tab = nt;
    A359598_tsz = nsz;
    for (size_t k = 0; k < A359598_cnt; ++k) {
        size_t s = (size_t)(A359598_hash(A359598_ex + k * A359598_MAXL, A359598_ln[k]) & (nsz - 1));
        while (A359598_tab[s] >= 0) s = (s + 1) & (nsz - 1);
        A359598_tab[s] = (int32_t)k;
    }
}

static long A359598_insert(const uint8_t *e, int len, seq_t mag, int neg)
{
    if (A359598_cnt + 1 > A359598_cap) {
        size_t nc = A359598_cap ? A359598_cap * 2 : 1024;
        seq_t *t = mem_resize(seq_t, A359598_mag, nc);
        if (!t) exit(EXIT_FAILURE);
        A359598_mag = t;
        A359598_neg = (uint8_t *)realloc(A359598_neg, nc);
        A359598_ln  = (uint8_t *)realloc(A359598_ln, nc);
        A359598_ex  = (uint8_t *)realloc(A359598_ex, nc * A359598_MAXL);
        if (!A359598_neg || !A359598_ln || !A359598_ex) exit(EXIT_FAILURE);
        A359598_cap = nc;
    }
    if ((A359598_cnt + 1) * 2 > A359598_tsz) A359598_rehash();
    size_t k = A359598_cnt++;
    A359598_mag[k] = mag;
    A359598_neg[k] = (uint8_t)neg;
    A359598_ln[k] = (uint8_t)len;
    memcpy(A359598_ex + k * A359598_MAXL, e, (size_t)len);
    size_t slot;
    A359598_find(e, len, &slot);
    A359598_tab[slot] = (int32_t)k;
    return (long)k;
}

/* returns index of the signature entry (exponents e, descending order not required) */
static long A359598_get(const uint8_t *e, int len)
{
    size_t slot;
    if (A359598_tsz) {
        long f = A359598_find(e, len, &slot);
        if (f >= 0) return f;
    }
    if (len == 0) return A359598_insert(e, 0, (seq_t)1, 0);

    uint8_t ee[A359598_MAXL];
    uint8_t c[A359598_MAXL];
    for (int i = 0; i < len; ++i) { ee[i] = e[i]; c[i] = 0; }
    seq_t P = 0, N = 0;
    for (;;) {
        int i = 0;
        while (i < len) {
            if (c[i] < ee[i]) { c[i]++; break; }
            c[i] = 0; ++i;
        }
        if (i == len) break;
        int sum = 0;
        for (int j = 0; j < len; ++j) sum += c[j];
        if (sum & 1) continue;
        uint8_t r[A359598_MAXL];
        int rl = 0;
        for (int j = 0; j < len; ++j) {
            uint8_t v = (uint8_t)(ee[j] - c[j]);
            if (v) r[rl++] = v;
        }
        for (int a = 1; a < rl; ++a) {
            uint8_t v = r[a];
            int b = a - 1;
            while (b >= 0 && r[b] < v) { r[b + 1] = r[b]; --b; }
            r[b + 1] = v;
        }
        long k = A359598_get(r, rl);
        if (A359598_neg[k]) N += A359598_mag[k]; else P += A359598_mag[k];
    }
    seq_t mag; int neg;
    if (N >= P) { mag = N - P; neg = 0; } else { mag = P - N; neg = 1; }
    return A359598_insert(ee, len, mag, neg);
}

typedef struct { idx_t m; long k; } A359598_cand;

static A359598_cand *A359598_cs = NULL;
static size_t A359598_cn = 0, A359598_ccap = 0;
static idx_t A359598_L = 0;
static unsigned A359598_pr[A359598_MAXL];

static void A359598_dfs(int pi, int maxe, idx_t m, uint8_t *ex, int depth)
{
    if (A359598_cn == A359598_ccap) {
        A359598_ccap = A359598_ccap ? A359598_ccap * 2 : 4096;
        A359598_cs = (A359598_cand *)realloc(A359598_cs, A359598_ccap * sizeof(A359598_cand));
        if (!A359598_cs) exit(EXIT_FAILURE);
    }
    A359598_cs[A359598_cn].m = m;
    A359598_cs[A359598_cn].k = A359598_get(ex, depth);
    A359598_cn++;
    if (pi >= A359598_MAXL - 1) return;
    idx_t p = A359598_pr[pi];
    idx_t cur = m;
    for (int e = 1; e <= maxe; ++e) {
        if (cur > A359598_L / p) break;
        cur *= p;
        ex[depth] = (uint8_t)e;
        A359598_dfs(pi + 1, e, cur, ex, depth + 1);
    }
}

static int A359598_cmp(const void *x, const void *y)
{
    idx_t a = ((const A359598_cand *)x)->m, b = ((const A359598_cand *)y)->m;
    return a < b ? -1 : (a > b ? 1 : 0);
}

static idx_t *A359598_rec = NULL;
static size_t A359598_nrec = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A359598_pr[0] == 0) {
        unsigned c = 3; int k = 0;
        while (k < A359598_MAXL) {
            bool ok = true;
            for (unsigned d = 3; d * d <= c; d += 2) if (c % d == 0) { ok = false; break; }
            if (ok) A359598_pr[k++] = c;
            c += 2;
        }
    }
    idx_t L = A359598_L ? A359598_L : ((idx_t)1 << 26);
    while (A359598_nrec < (size_t)n) {
        if (L > ((idx_t)1 << 61)) return 0;
        A359598_L = L;
        A359598_cn = 0;
        uint8_t ex[A359598_MAXL];
        A359598_dfs(0, 200, 1, ex, 0);
        qsort(A359598_cs, A359598_cn, sizeof(A359598_cand), A359598_cmp);
        free(A359598_rec);
        A359598_rec = (idx_t *)malloc((A359598_cn + 1) * sizeof(idx_t));
        if (!A359598_rec) exit(EXIT_FAILURE);
        A359598_nrec = 0;
        seq_t best = 0;
        for (size_t i = 0; i < A359598_cn; ++i) {
            seq_t mg = A359598_mag[A359598_cs[i].k];
            if (mg > best) {
                best = mg;
                A359598_rec[A359598_nrec++] = A359598_cs[i].m;
            }
        }
        if (A359598_nrec < (size_t)n) L <<= 2;
    }
    return (seq_t)A359598_rec[n - 1];
}
