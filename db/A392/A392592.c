/*
 * A392592  Intersection of A391845 and A391866.
 * https://oeis.org/A392592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k is a term iff A053669(A003415(k)) = A020639(k), k is
 *            not squarefree, no prime p has p^(p+1) | k (A359550(k) = 1), and
 *            A003557(k) = k/rad(k) divides A276085(k) = Sum e_i * (product of
 *            primes < p_i) over k = Prod p_i^e_i. Numbers are factored by a
 *            segmented sieve. The A276085 test is done...
 * Range:     u8 n <= 1, u16 n <= 216, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9212 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint32_t *A392592_pr = 0;
static size_t A392592_np = 0;
static uint64_t A392592_plim = 0;

static void A392592_ensure(uint64_t L)
{
    if (A392592_pr && L <= A392592_plim) return;
    uint64_t nl = A392592_plim * 2;
    if (nl < L) nl = L;
    if (nl < 65536) nl = 65536;
    unsigned char *s = (unsigned char *)calloc((size_t)(nl + 1), 1);
    size_t cnt = 0;
    for (uint64_t i = 2; i <= nl; i++) {
        if (!s[i]) {
            cnt++;
            for (uint64_t j = i * i; j <= nl; j += i) s[j] = 1;
        }
    }
    uint32_t *np = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t c = 0;
    for (uint64_t i = 2; i <= nl; i++) if (!s[i]) np[c++] = (uint32_t)i;
    free(s);
    if (A392592_pr) free(A392592_pr);
    A392592_pr = np;
    A392592_np = cnt;
    A392592_plim = nl;
}

static size_t A392592_index(uint64_t q)
{
    size_t lo = 0, hi = A392592_np;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (A392592_pr[mid] < q) lo = mid + 1; else hi = mid;
    }
    return lo;
}

static uint64_t A392592_mm(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

/* product of the first idx primes, mod Q, by direct loop */
static uint64_t A392592_direct(size_t idx, uint64_t Q)
{
    uint64_t r = 1 % Q;
    for (size_t j = 0; j < idx; j++) r = A392592_mm(r, A392592_pr[j], Q);
    return r;
}

typedef struct { uint64_t Q; uint64_t *cp; size_t len, cap; } A392592_cpent;
static A392592_cpent *A392592_cps = 0;
static size_t A392592_ncp = 0, A392592_capcp = 0;

/* product of the first idx primes, mod Q, using checkpoints every 64 primes */
static uint64_t A392592_pmod(size_t idx, uint64_t Q)
{
    if (idx < 128) return A392592_direct(idx, Q);
    A392592_cpent *e = 0;
    for (size_t i = 0; i < A392592_ncp; i++)
        if (A392592_cps[i].Q == Q) { e = &A392592_cps[i]; break; }
    if (!e) {
        if (A392592_ncp == A392592_capcp) {
            A392592_capcp = A392592_capcp ? A392592_capcp * 2 : 16;
            A392592_cps = (A392592_cpent *)realloc(A392592_cps, A392592_capcp * sizeof(A392592_cpent));
        }
        e = &A392592_cps[A392592_ncp++];
        e->Q = Q; e->cap = 64; e->len = 1;
        e->cp = (uint64_t *)malloc(e->cap * sizeof(uint64_t));
        e->cp[0] = 1 % Q;
    }
    size_t j = idx / 64;
    while (e->len <= j) {
        uint64_t b = e->cp[e->len - 1];
        for (size_t t = 64 * (e->len - 1); t < 64 * e->len; t++) b = A392592_mm(b, A392592_pr[t], Q);
        if (e->len == e->cap) {
            e->cap *= 2;
            e->cp = (uint64_t *)realloc(e->cp, e->cap * sizeof(uint64_t));
        }
        e->cp[e->len++] = b;
    }
    uint64_t r = e->cp[j];
    for (size_t t = 64 * j; t < idx; t++) r = A392592_mm(r, A392592_pr[t], Q);
    return r;
}

static int A392592_test(uint64_t k, const uint64_t *fp, const unsigned char *fe, int nf)
{
    int nonsf = 0;
    for (int i = 0; i < nf; i++) {
        if ((uint64_t)fe[i] > fp[i]) return 0;
        if (fe[i] >= 2) nonsf = 1;
    }
    if (!nonsf) return 0;
    uint64_t d = 0;
    for (int i = 0; i < nf; i++) d += (uint64_t)fe[i] * (k / fp[i]);
    uint64_t snd = 0;
    for (size_t i = 0; i < A392592_np; i++) {
        uint64_t p = A392592_pr[i];
        if (d % p != 0) { snd = p; break; }
    }
    if (snd != fp[0]) return 0;
    size_t idx[16];
    for (int i = 0; i < nf; i++) idx[i] = A392592_index(fp[i]);
    for (int j = 0; j < nf; j++) {
        if (fe[j] < 2) continue;
        uint64_t q = fp[j];
        int f = fe[j] - 1;
        uint64_t Q = 1;
        for (int t = 0; t < f; t++) Q *= q;
        uint64_t s = 0;
        for (int i = 0; i < nf; i++) {
            uint64_t c;
            if (fp[i] <= q) c = A392592_direct(idx[i], Q);
            else if (f == 1) c = 0;
            else c = A392592_pmod(idx[i], Q);
            s = (s + A392592_mm((uint64_t)fe[i] % Q, c, Q)) % Q;
        }
        if (s != 0) return 0;
    }
    return 1;
}

static uint64_t *A392592_found = 0;
static size_t A392592_cnt = 0, A392592_cap = 0;
static uint64_t A392592_lo = 2;

#define A392592_SEG 32768

static void A392592_segment(void)
{
    uint64_t lo = A392592_lo, hi = lo + A392592_SEG;
    A392592_ensure(hi + 1);
    size_t S = (size_t)(hi - lo);
    uint64_t *rem = (uint64_t *)malloc(S * sizeof(uint64_t));
    uint64_t *fp = (uint64_t *)malloc(S * 16 * sizeof(uint64_t));
    unsigned char *fe = (unsigned char *)malloc(S * 16);
    unsigned char *nf = (unsigned char *)calloc(S, 1);
    for (size_t i = 0; i < S; i++) rem[i] = lo + i;
    for (size_t t = 0; t < A392592_np; t++) {
        uint64_t p = A392592_pr[t];
        if (p * p >= hi) break;
        uint64_t st = ((lo + p - 1) / p) * p;
        for (uint64_t k = st; k < hi; k += p) {
            size_t i = (size_t)(k - lo);
            int e = 0;
            while (rem[i] % p == 0) { rem[i] /= p; e++; }
            fp[i * 16 + nf[i]] = p;
            fe[i * 16 + nf[i]] = (unsigned char)e;
            nf[i]++;
        }
    }
    for (size_t i = 0; i < S; i++) {
        if (rem[i] > 1) {
            fp[i * 16 + nf[i]] = rem[i];
            fe[i * 16 + nf[i]] = 1;
            nf[i]++;
        }
        uint64_t k = lo + i;
        if (k < 4) continue;
        if (A392592_test(k, fp + i * 16, fe + i * 16, nf[i])) {
            if (A392592_cnt == A392592_cap) {
                A392592_cap = A392592_cap ? A392592_cap * 2 : 256;
                A392592_found = (uint64_t *)realloc(A392592_found, A392592_cap * sizeof(uint64_t));
            }
            A392592_found[A392592_cnt++] = k;
        }
    }
    free(rem); free(fp); free(fe); free(nf);
    A392592_lo = hi;
}

seq_t a(idx_t n)
{
    if (n == 0) n = 1;
    while (A392592_cnt < (size_t)n) A392592_segment();
    return (seq_t)A392592_found[n - 1];
}
