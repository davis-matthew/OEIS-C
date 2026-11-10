/*
 * A271048  Numbers n such that A000070(n) is prime.
 * https://oeis.org/A271048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th m >= 1 such that A000070(m) = Sum_{k=0..m}
 *            p(k) is prime. p(k) comes from Euler's pentagonal recurrence,
 *            computed in multiprecision. Primality: trial division, then
 *            Miller-Rabin with Montgomery multiplication
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t **A271048_Pd = 0;
static int *A271048_Pl = 0;
static idx_t A271048_Pcap = 0;
static idx_t A271048_Phave = 0;
static uint32_t *A271048_Sd = 0;
static int A271048_Sl = 0;
static int A271048_Scap = 0;
static idx_t *A271048_terms = 0;
static size_t A271048_cnt = 0;
static size_t A271048_tcap = 0;

static void A271048_add_to(uint32_t *acc, int *al, const uint32_t *x, int xl)
{
    int n = (*al > xl) ? *al : xl, i;
    uint64_t c = 0;
    for (i = 0; i < n; i++) {
        uint64_t s = c;
        if (i < *al) s += acc[i];
        if (i < xl) s += x[i];
        acc[i] = (uint32_t)s;
        c = s >> 32;
    }
    if (c) acc[n++] = (uint32_t)c;
    *al = n;
}

static void A271048_sub_from(uint32_t *acc, int *al, const uint32_t *x, int xl)
{
    int i;
    int64_t br = 0;
    for (i = 0; i < *al; i++) {
        int64_t d = (int64_t)acc[i] - br - (i < xl ? (int64_t)x[i] : 0);
        if (d < 0) { d += ((int64_t)1 << 32); br = 1; } else br = 0;
        acc[i] = (uint32_t)d;
    }
    while (*al > 0 && acc[*al - 1] == 0) (*al)--;
}

static int A271048_cmp(const uint32_t *a, const uint32_t *b, int L)
{
    int i;
    for (i = L - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void A271048_subN(uint32_t *x, const uint32_t *N, int L)
{
    int i;
    int64_t br = 0;
    for (i = 0; i < L; i++) {
        int64_t d = (int64_t)x[i] - br - (int64_t)N[i];
        if (d < 0) { d += ((int64_t)1 << 32); br = 1; } else br = 0;
        x[i] = (uint32_t)d;
    }
}

static void A271048_dblmod(uint32_t *x, const uint32_t *N, int L)
{
    uint32_t carry = x[L - 1] >> 31;
    int i;
    for (i = L - 1; i > 0; i--) x[i] = (x[i] << 1) | (x[i - 1] >> 31);
    x[0] <<= 1;
    if (carry || A271048_cmp(x, N, L) >= 0) A271048_subN(x, N, L);
}

static void A271048_mont(uint32_t *r, const uint32_t *a, const uint32_t *b,
                         const uint32_t *N, int L, uint32_t ninv, uint32_t *t)
{
    int i, j;
    for (j = 0; j < L + 2; j++) t[j] = 0;
    for (i = 0; i < L; i++) {
        uint64_t C = 0, s;
        uint32_t m;
        for (j = 0; j < L; j++) {
            s = (uint64_t)t[j] + (uint64_t)a[j] * b[i] + C;
            t[j] = (uint32_t)s;
            C = s >> 32;
        }
        s = (uint64_t)t[L] + C;
        t[L] = (uint32_t)s;
        t[L + 1] = (uint32_t)(s >> 32);
        m = t[0] * ninv;
        s = (uint64_t)t[0] + (uint64_t)m * N[0];
        C = s >> 32;
        for (j = 1; j < L; j++) {
            s = (uint64_t)t[j] + (uint64_t)m * N[j] + C;
            t[j - 1] = (uint32_t)s;
            C = s >> 32;
        }
        s = (uint64_t)t[L] + C;
        t[L - 1] = (uint32_t)s;
        t[L] = t[L + 1] + (uint32_t)(s >> 32);
    }
    if (t[L] || A271048_cmp(t, N, L) >= 0) A271048_subN(t, N, L);
    for (j = 0; j < L; j++) r[j] = t[j];
}

static bool A271048_mr(const uint32_t *N, int L)
{
    static const uint32_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint32_t inv = 1, ninv;
    uint32_t *oneM = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint32_t *mone = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint32_t *d = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint32_t *bm = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint32_t *y = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc((size_t)L + 2, sizeof(uint32_t));
    int i, k, s = 0, bits, bi;
    bool result = true;

    for (i = 0; i < 5; i++) inv *= 2u - N[0] * inv;
    ninv = (uint32_t)(0u - inv);

    oneM[0] = 1;
    for (i = 0; i < 32 * L; i++) A271048_dblmod(oneM, N, L);
    for (i = 0; i < L; i++) mone[i] = N[i];
    A271048_subN(mone, oneM, L); /* N - R mod N */

    for (i = 0; i < L; i++) d[i] = N[i];
    d[0] -= 1;
    for (;;) {
        if (d[0] & 1u) break;
        for (i = 0; i < L - 1; i++) d[i] = (d[i] >> 1) | (d[i + 1] << 31);
        d[L - 1] >>= 1;
        s++;
    }
    bits = 32 * L;
    while (bits > 0 && !((d[(bits - 1) / 32] >> ((bits - 1) % 32)) & 1u)) bits--;

    for (k = 0; k < 12 && result; k++) {
        bool ok = false;
        for (i = 0; i < L; i++) bm[i] = 0;
        bm[0] = bases[k];
        for (i = 0; i < 32 * L; i++) A271048_dblmod(bm, N, L);
        for (i = 0; i < L; i++) y[i] = bm[i];
        for (bi = bits - 2; bi >= 0; bi--) {
            A271048_mont(y, y, y, N, L, ninv, t);
            if ((d[bi / 32] >> (bi % 32)) & 1u) A271048_mont(y, y, bm, N, L, ninv, t);
        }
        if (A271048_cmp(y, oneM, L) == 0 || A271048_cmp(y, mone, L) == 0) continue;
        for (i = 1; i < s; i++) {
            A271048_mont(y, y, y, N, L, ninv, t);
            if (A271048_cmp(y, mone, L) == 0) { ok = true; break; }
            if (A271048_cmp(y, oneM, L) == 0) break;
        }
        if (!ok) result = false;
    }
    free(oneM); free(mone); free(d); free(bm); free(y); free(t);
    return result;
}

static bool A271048_isprime(const uint32_t *x, int L)
{
    uint32_t q;
    if (L == 1) {
        uint64_t v = x[0], i;
        if (v < 2) return false;
        for (i = 2; i * i <= v; i++)
            if (v % i == 0) return false;
        return true;
    }
    for (q = 2; q < 2000; q++) {
        uint64_t r = 0;
        int i;
        for (i = L - 1; i >= 0; i--) r = ((r << 32) | x[i]) % q;
        if (r == 0) return false;
    }
    return A271048_mr(x, L);
}

static void A271048_step(void)
{
    idx_t m = A271048_Phave, k;
    int pl;
    uint32_t *pv;
    if (m + 1 > A271048_Pcap) {
        idx_t nc = A271048_Pcap ? A271048_Pcap * 2 : 256;
        uint32_t **nd = (uint32_t **)malloc((size_t)nc * sizeof(uint32_t *));
        int *nl = (int *)malloc((size_t)nc * sizeof(int));
        idx_t i;
        for (i = 0; i < A271048_Phave; i++) { nd[i] = A271048_Pd[i]; nl[i] = A271048_Pl[i]; }
        free(A271048_Pd); free(A271048_Pl);
        A271048_Pd = nd; A271048_Pl = nl; A271048_Pcap = nc;
    }
    if (m == 0) {
        pv = (uint32_t *)malloc(sizeof(uint32_t));
        pv[0] = 1;
        pl = 1;
    } else {
        int bs = A271048_Pl[m - 1] + 3, pol = 0, ngl = 0;
        uint32_t *pos = (uint32_t *)calloc((size_t)bs, sizeof(uint32_t));
        uint32_t *neg = (uint32_t *)calloc((size_t)bs, sizeof(uint32_t));
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            uint32_t *acc;
            int *al;
            if (g1 > m) break;
            if (k & 1) { acc = pos; al = &pol; } else { acc = neg; al = &ngl; }
            A271048_add_to(acc, al, A271048_Pd[m - g1], A271048_Pl[m - g1]);
            if (g2 <= m) A271048_add_to(acc, al, A271048_Pd[m - g2], A271048_Pl[m - g2]);
        }
        A271048_sub_from(pos, &pol, neg, ngl);
        pl = pol;
        pv = (uint32_t *)malloc((size_t)pl * sizeof(uint32_t));
        { int i; for (i = 0; i < pl; i++) pv[i] = pos[i]; }
        free(pos); free(neg);
    }
    A271048_Pd[m] = pv;
    A271048_Pl[m] = pl;
    A271048_Phave = m + 1;

    {
        int need = ((A271048_Sl > pl) ? A271048_Sl : pl) + 1;
        if (need > A271048_Scap) {
            int nc = need * 2, i;
            uint32_t *ns = (uint32_t *)calloc((size_t)nc, sizeof(uint32_t));
            for (i = 0; i < A271048_Sl; i++) ns[i] = A271048_Sd[i];
            free(A271048_Sd);
            A271048_Sd = ns;
            A271048_Scap = nc;
        }
        A271048_add_to(A271048_Sd, &A271048_Sl, pv, pl);
    }

    if (m >= 1 && A271048_isprime(A271048_Sd, A271048_Sl)) {
        if (A271048_cnt >= A271048_tcap) {
            size_t nc = A271048_tcap ? A271048_tcap * 2 : 64, i;
            idx_t *nt = (idx_t *)malloc(nc * sizeof(idx_t));
            for (i = 0; i < A271048_cnt; i++) nt[i] = A271048_terms[i];
            free(A271048_terms);
            A271048_terms = nt;
            A271048_tcap = nc;
        }
        A271048_terms[A271048_cnt++] = m;
    }
}

seq_t a(idx_t n)
{
    while (A271048_cnt < (size_t)n) A271048_step();
    return (seq_t)A271048_terms[n - 1];
}
