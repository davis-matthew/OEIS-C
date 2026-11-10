/*
 * A360912  Records in A355432.
 * https://oeis.org/A360912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over n = 2, 3, ...; a(n) is the n-th record value of
 *            A355432(m) = * #{t <= m/rad(m) - 1 : t has only prime factors of
 *            m} - (tau(m/rad(m)) - 1). * m/rad(m) comes from a segmented
 *            sieve over prime powers
 * Range:     u8 n <= 58, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A360912_BLK 262144u
#define A360912_MAXREC 4096

static uint64_t A360912_recs[A360912_MAXREC];
static size_t A360912_cnt = 0;
static uint64_t A360912_next = 2;
static uint64_t A360912_best = 0;
static uint32_t *A360912_pr = 0;
static size_t A360912_npr = 0;
static uint64_t A360912_plim = 0;
static uint64_t A360912_exc[A360912_BLK];

static void A360912_build_primes(uint64_t lim)
{
    uint64_t L = lim * 2 + 100;
    unsigned char *s = (unsigned char *)calloc((size_t)L + 1, 1);
    size_t c = 0;
    uint64_t i, j;
    free(A360912_pr);
    A360912_pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)L / 2 + 10));
    for (i = 2; i <= L; i++) {
        if (!s[i]) {
            A360912_pr[c++] = (uint32_t)i;
            for (j = i * i; j <= L; j += i) s[j] = 1;
        }
    }
    free(s);
    A360912_npr = c;
    A360912_plim = L;
}

static void A360912_dfs(const uint64_t *pr, size_t cnt, size_t idx,
                        uint64_t cur, uint64_t limit, uint64_t *total)
{
    if (idx == cnt) { (*total)++; return; }
    uint64_t p = pr[idx];
    uint64_t v = cur;
    while (v <= limit) {
        A360912_dfs(pr, cnt, idx + 1, v, limit, total);
        if (v > limit / p) break;
        v *= p;
    }
}

/* value of A355432(n) given E = n / rad(n) */
static uint64_t A360912_value(uint64_t n, uint64_t E)
{
    uint64_t pr[64];
    size_t cnt = 0;
    uint64_t tau = 1;
    uint64_t e = E, r = n;
    size_t k;
    /* factor E: its primes are those p with p^2 | n */
    for (k = 0; k < A360912_npr; k++) {
        uint64_t p = A360912_pr[k];
        if (p * p > e) break;
        if (e % p == 0) {
            uint64_t f = 0;
            while (e % p == 0) { e /= p; f++; }
            pr[cnt++] = p;
            tau *= f + 1;
        }
    }
    if (e > 1) {
        bool dup = false;
        for (k = 0; k < cnt; k++) if (pr[k] == e) dup = true;
        if (!dup) { pr[cnt++] = e; tau *= 2; }
        else {
            /* cannot happen: remaining e is a prime not yet seen */
        }
    }
    /* recompute tau(E) exactly (e > 1 case handled above) */
    for (k = 0; k < cnt; k++)
        while (r % pr[k] == 0) r /= pr[k];
    /* r is squarefree; only its primes < E can appear in t <= E-1 */
    for (k = 0; k < A360912_npr && r > 1; k++) {
        uint64_t p = A360912_pr[k];
        if (p >= E) break;
        if (p * p > r) break;
        if (r % p == 0) { pr[cnt++] = p; r /= p; }
    }
    if (r > 1 && r < E) pr[cnt++] = r;
    uint64_t total = 0;
    A360912_dfs(pr, cnt, 0, 1, E - 1, &total);
    return total - (tau - 1);
}

static void A360912_extend(void)
{
    uint64_t lo = A360912_next;
    uint64_t hi = lo + A360912_BLK;
    uint64_t sq = 1;
    uint64_t i;
    while (sq * sq <= hi) sq++;
    if (sq + 1 > A360912_plim) A360912_build_primes(sq + 1);
    for (i = 0; i < A360912_BLK; i++) A360912_exc[i] = 1;
    for (size_t k = 0; k < A360912_npr; k++) {
        uint64_t p = A360912_pr[k];
        if (p * p >= hi) break;
        uint64_t pk = p * p;
        while (pk < hi) {
            uint64_t m = ((lo + pk - 1) / pk) * pk;
            for (; m < hi; m += pk) A360912_exc[m - lo] *= p;
            if (pk > hi / p) break;
            pk *= p;
        }
    }
    for (i = 0; i < A360912_BLK; i++) {
        uint64_t E = A360912_exc[i];
        if (E <= A360912_best + 1) continue;   /* value <= E - 1 */
        uint64_t n = lo + i;
        uint64_t v = A360912_value(n, E);
        if (v > A360912_best) {
            A360912_best = v;
            if (A360912_cnt < A360912_MAXREC) A360912_recs[A360912_cnt++] = v;
        }
    }
    A360912_next = hi;
}

seq_t a(idx_t n)
{
    if (A360912_cnt == 0) {
        A360912_recs[0] = 0;   /* A355432(1) = 0 */
        A360912_cnt = 1;
    }
    while (A360912_cnt < n && A360912_cnt < A360912_MAXREC)
        A360912_extend();
    return (seq_t)A360912_recs[n - 1];
}
