/*
 * A332445  Numbers k of the form 4m+1 for which A087808(sigma(k)) is equal to
 *          2*A087808(k).
 * https://oeis.org/A332445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. By 2-adic valuation, sigma(k) == 2 (mod 4) or k is an
 *            odd square, so k = p^e*m^2 with p == 1 (mod 4), e == 1 (mod 4),
 *            p not dividing m (m=1,e=1 never works). Candidates are
 *            enumerated with a segmented prime sieve and tested for
 *            A087808(sigma(k)) = 2*A087808(k)
 * Range:     u8 none, u16 n <= 4, u32 n <= 218, u64 n <= 299, u128 n <= 299,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 281 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint64_t A087808_u64(uint64_t x)
{
    uint64_t r = 0;
    unsigned ones = 0;
    while (x) {
        unsigned i = (unsigned)__builtin_ctzll(x);
        r += (uint64_t)1 << (i - ones);
        ones++;
        x &= x - 1;
    }
    return r;
}

static uint64_t my_isqrt(uint64_t x)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x) {
        uint64_t step = 1;
        while ((r + 2 * step) * (r + 2 * step) <= x) step *= 2;
        r += step;
    }
    return r;
}

/* odd base primes and residue start for q = 4i+1 divisible by r */
static uint64_t *bp = NULL, *bp_j0 = NULL;
static size_t bp_cnt = 0;
static uint64_t bp_lim = 0;

static void ensure_bp(uint64_t lim)
{
    if (lim <= bp_lim) return;
    uint64_t nl = bp_lim ? bp_lim * 2 : 1024;
    while (nl < lim) nl *= 2;
    unsigned char *s = (unsigned char *)calloc((size_t)nl + 1, 1);
    free(bp); free(bp_j0);
    bp = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)nl / 2 + 8));
    bp_j0 = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)nl / 2 + 8));
    bp_cnt = 0;
    for (uint64_t i = 2; i <= nl; i++) {
        if (!s[i]) {
            for (uint64_t j = i * i; j <= nl; j += i) s[j] = 1;
            if (i > 2) {
                uint64_t inv2 = (i + 1) / 2;
                uint64_t inv4 = (inv2 * inv2) % i;
                bp[bp_cnt] = i;
                bp_j0[bp_cnt] = (i - inv4) % i;
                bp_cnt++;
            }
        }
    }
    free(s);
    bp_lim = nl;
}

/* cache of sigma(m^2) */
static uint64_t *ssq = NULL;
static uint64_t ssq_lim = 0; /* valid for m < ssq_lim */

static void ensure_ssq(uint64_t M)
{
    if (M < ssq_lim) return;
    uint64_t nl = ssq_lim ? ssq_lim * 2 : 1024;
    while (nl <= M) nl *= 2;
    ensure_bp(my_isqrt(nl) + 2);
    ssq = (uint64_t *)realloc(ssq, sizeof(uint64_t) * (size_t)nl);
    for (uint64_t m = ssq_lim; m < nl; m++) {
        if (m == 0) { ssq[0] = 0; continue; }
        uint64_t x = m, res = 1;
        if ((x & 1) == 0) {
            uint64_t sum = 1, pw = 1;
            while ((x & 1) == 0) { x >>= 1; pw *= 4; sum += pw / 2 + pw; }
            res *= sum;
        }
        for (size_t i = 0; i < bp_cnt && bp[i] * bp[i] <= x; i++) {
            uint64_t p = bp[i];
            if (x % p == 0) {
                uint64_t sum = 1, pw = 1;
                while (x % p == 0) { x /= p; pw *= p; sum += pw; pw *= p; sum += pw; }
                res *= sum;
            }
        }
        if (x > 1) res *= 1 + x + x * x;
        ssq[m] = res;
    }
    ssq_lim = nl;
}

static uint64_t *chunk = NULL;
static size_t ccnt = 0, ccap = 0;

static void push_chunk(uint64_t k)
{
    if (ccnt == ccap) {
        ccap = ccap ? ccap * 2 : 256;
        chunk = (uint64_t *)realloc(chunk, sizeof(uint64_t) * ccap);
    }
    chunk[ccnt++] = k;
}

#define SEG 65536u
static unsigned char mark[SEG];

static void scan_m(uint64_t m, uint64_t L, uint64_t R)
{
    uint64_t m2 = m * m, S = ssq[m];
    uint64_t qa = L / m2, qb = R / m2;
    if (qb < 5 || qb <= qa) return;
    uint64_t i0 = (qa + 3) / 4, i1 = (qb - 1) / 4;
    if (i0 == 0) i0 = 1;
    for (uint64_t ia = i0; ia <= i1; ia += SEG) {
        uint64_t ib = ia + SEG;
        if (ib > i1 + 1) ib = i1 + 1;
        memset(mark, 0, (size_t)(ib - ia));
        uint64_t qmax = 4 * (ib - 1) + 1;
        for (size_t t = 0; t < bp_cnt; t++) {
            uint64_t r = bp[t];
            if (r * r > qmax) break;
            uint64_t st = (r * r - 1) / 4;
            if (st < ia) st = ia;
            uint64_t s = st + (bp_j0[t] + r - st % r) % r;
            for (uint64_t j = s; j < ib; j += r) mark[j - ia] = 1;
        }
        for (uint64_t i = ia; i < ib; i++) {
            if (mark[i - ia]) continue;
            uint64_t q = 4 * i + 1;
            if (q <= m && m % q == 0) continue;
            uint64_t k = q * m2;
            uint64_t tt = ((q + 1) / 2) * S;
            if ((((unsigned)__builtin_ctzll(~tt)) ^ ((unsigned)__builtin_ctzll(~k))) & 1u) continue;
            if (A087808_u64(tt) == A087808_u64(k)) push_chunk(k);
        }
    }
}

static int cmp_u64(const void *x, const void *y)
{
    uint64_t a1 = *(const uint64_t *)x, b1 = *(const uint64_t *)y;
    return (a1 > b1) - (a1 < b1);
}

static void process_range(uint64_t L, uint64_t R)
{
    uint64_t sr = my_isqrt(R);
    ensure_bp(sr + 2);
    ensure_ssq(sr + 1);
    ccnt = 0;
    /* odd squares */
    for (uint64_t m = 1; m * m <= R; m += 2) {
        uint64_t k = m * m;
        if (k <= L) continue;
        if (A087808_u64(ssq[m]) == 2 * A087808_u64(k)) push_chunk(k);
    }
    /* p^e * m^2, e = 5, 9, ... */
    for (size_t t = 0; t < bp_cnt; t++) {
        uint64_t p = bp[t];
        if ((p & 3) != 1) continue;
        uint64_t p4 = p * p * p * p;
        if (p4 > R / p) break;
        uint64_t pe = p4 * p, sp = 1, pw = 1;
        for (int e = 0; e < 5; e++) { pw *= p; sp += pw; }
        for (;;) {
            for (uint64_t m = 1; pe * m * m <= R; m += 2) {
                uint64_t k = pe * m * m;
                if (k <= L || m % p == 0) continue;
                uint64_t s = sp * ssq[m];
                if (A087808_u64(s) == 2 * A087808_u64(k)) push_chunk(k);
            }
            if (pe > R / p4) break;
            pe *= p4;
            for (int e = 0; e < 4; e++) { pw *= p; sp += pw; }
        }
    }
    /* p * m^2, m >= 3 odd */
    for (uint64_t m = 3; 5 * m * m <= R; m += 2) scan_m(m, L, R);
    if (ccnt > 1) qsort(chunk, ccnt, sizeof(uint64_t), cmp_u64);
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t tcnt = 0, tcap = 0;
    static uint64_t L = 0;
    while (tcnt < (size_t)n) {
        uint64_t step = L / 4;
        if (step < ((uint64_t)1 << 22)) step = (uint64_t)1 << 22;
        uint64_t R = L + step;
        process_range(L, R);
        for (size_t i = 0; i < ccnt; i++) {
            if (tcnt == tcap) {
                tcap = tcap ? tcap * 2 : 64;
                terms = (uint64_t *)realloc(terms, sizeof(uint64_t) * tcap);
            }
            terms[tcnt++] = chunk[i];
        }
        L = R;
    }
    return (seq_t)terms[n - 1];
}
