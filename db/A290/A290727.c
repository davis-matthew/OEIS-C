/*
 * A290727  Analog of A085635, replacing "quadratic residue" (X^2) with "value
 *          of X^2+X".
 * https://oeis.org/A290727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (k = 1, 2, ...) with A290731(k)/k < A290731(j)/j
 *            for all 1 <= j < k. Computed by a Pareto-frontier DP over primes
 *            (A290731 is multiplicative): keep only numbers whose ratio is
 *            strictly below that of every smaller number
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 116, u64 n <= 116,
 *            u128 n <= 116, big any n
 * Validated: standard, pass, 56 terms through n = 112 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

typedef unsigned __int128 a290727_u128;
typedef struct { a290727_u128 k; a290727_u128 A; } a290727_node;

static a290727_node *a290727_rec_nodes = NULL;
static a290727_u128 *a290727_rec = NULL;
static size_t a290727_cnt = 0;
static int a290727_stage = 0;

static void a290727_mul128(a290727_u128 a, a290727_u128 b, a290727_u128 *hi, a290727_u128 *lo)
{
    const a290727_u128 M = (a290727_u128)0xFFFFFFFFFFFFFFFFULL;
    a290727_u128 a0 = a & M, a1 = a >> 64, b0 = b & M, b1 = b >> 64;
    a290727_u128 p00 = a0 * b0, p01 = a0 * b1, p10 = a1 * b0, p11 = a1 * b1;
    a290727_u128 mid = (p00 >> 64) + (p01 & M) + (p10 & M);
    *lo = (p00 & M) | (mid << 64);
    *hi = p11 + (p01 >> 64) + (p10 >> 64) + (mid >> 64);
}

/* true if A1/k1 < A2/k2 */
static bool a290727_less(a290727_u128 A1, a290727_u128 k1, a290727_u128 A2, a290727_u128 k2)
{
    a290727_u128 h1, l1, h2, l2;
    a290727_mul128(A1, k2, &h1, &l1);
    a290727_mul128(A2, k1, &h2, &l2);
    if (h1 != h2) return h1 < h2;
    return l1 < l2;
}

static int a290727_cmp(const void *x, const void *y)
{
    const a290727_node *a = (const a290727_node *)x;
    const a290727_node *b = (const a290727_node *)y;
    if (a->k < b->k) return -1;
    if (a->k > b->k) return 1;
    return 0;
}

/* A290731(p^e) given P = p^e */
static a290727_u128 a290727_ap(uint64_t p, int e, a290727_u128 P)
{
    if (e == 0) return 1;
    if (p == 2) return P / 2;
    a290727_u128 d = (a290727_u128)2 * (p + 1);
    a290727_u128 q1 = P / d, r1 = P % d;
    return 1 + q1 * p + (r1 * p) / d;
}

static void a290727_build(int bits, unsigned L)
{
    a290727_u128 X = ((a290727_u128)1) << bits;
    unsigned char *comp = (unsigned char *)calloc(L + 1, 1);
    size_t capa = 16, capb = 16, nf = 1;
    a290727_node *cur = (a290727_node *)malloc(capa * sizeof(a290727_node));
    a290727_node *tmp = (a290727_node *)malloc(capb * sizeof(a290727_node));
    a290727_u128 pw[130], apw[130];
    cur[0].k = 1; cur[0].A = 1;

    for (unsigned p = 2; p <= L; ++p) {
        if (comp[p]) continue;
        for (unsigned long long q = (unsigned long long)p * p; q <= L; q += p) comp[q] = 1;
        if ((a290727_u128)p > X) break;
        int emax = 0;
        pw[0] = 1;
        while (pw[emax] <= X / p) { pw[emax + 1] = pw[emax] * p; ++emax; }
        for (int e = 0; e <= emax; ++e) apw[e] = a290727_ap(p, e, pw[e]);
        size_t need = nf * (size_t)(emax + 1);
        if (need > capb) {
            capb = need;
            tmp = (a290727_node *)realloc(tmp, capb * sizeof(a290727_node));
        }
        size_t m = 0;
        for (size_t i = 0; i < nf; ++i) {
            a290727_u128 lim = X / cur[i].k;
            for (int e = 0; e <= emax; ++e) {
                if (pw[e] > lim) break;
                tmp[m].k = cur[i].k * pw[e];
                tmp[m].A = cur[i].A * apw[e];
                ++m;
            }
        }
        qsort(tmp, m, sizeof(a290727_node), a290727_cmp);
        size_t nn = 0;
        a290727_u128 bA = 0, bk = 1;
        for (size_t i = 0; i < m; ++i) {
            if (nn == 0 || a290727_less(tmp[i].A, tmp[i].k, bA, bk)) {
                bA = tmp[i].A; bk = tmp[i].k;
                tmp[nn++] = tmp[i];
            }
        }
        a290727_node *t = cur; cur = tmp; tmp = t;
        size_t tc = capa; capa = capb; capb = tc;
        nf = nn;
    }

    if (a290727_rec) free(a290727_rec);
    a290727_rec = (a290727_u128 *)malloc((nf + 1) * sizeof(a290727_u128));
    for (size_t i = 0; i < nf; ++i) a290727_rec[i] = cur[i].k;
    a290727_cnt = nf;
    free(cur);
    free(tmp);
    free(comp);
}

static seq_t a290727_conv(a290727_u128 v)
{
    uint64_t hi = (uint64_t)(v >> 64);
    uint64_t lo = (uint64_t)v;
    seq_t res = 0;
    if (hi != 0) {
        res = (seq_t)hi;
        res *= 65536; res *= 65536; res *= 65536; res *= 65536;
    }
    res += (seq_t)lo;
    return res;
}

seq_t a(idx_t n)
{
    static const int bits_tab[4] = {32, 64, 100, 126};
    static const unsigned lim_tab[4] = {1200, 3700, 7500, 11100};
    if (n == 0) return 0;
    while ((size_t)n > a290727_cnt && a290727_stage < 4) {
        a290727_build(bits_tab[a290727_stage], lim_tab[a290727_stage]);
        ++a290727_stage;
    }
    if ((size_t)n > a290727_cnt) return 0;
    return a290727_conv(a290727_rec[n - 1]);
}
