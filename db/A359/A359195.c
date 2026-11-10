/*
 * A359195  Positive integers k with a smaller fraction of powers (mod k) than
 *          any smaller positive integers.
 * https://oeis.org/A359195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th record of the fraction of residues mod k that are
 *            NOT powers y^e (e>=2); search over products of primorials
 *            (A025487), per the conjecture
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static const int SP[30] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113};

static void gen_cand(seq_t cur, int i, int maxe, seq_t lim, seq_t *out, size_t *cnt) {
    if (out) out[*cnt] = cur;
    (*cnt)++;
    if (i >= 30) return;
    seq_t p = (seq_t)SP[i];
    seq_t c = cur;
    for (int e = 1; e <= maxe; e++) {
        if (c > lim / p) break;
        c *= p;
        gen_cand(c, i + 1, e, lim, out, cnt);
    }
}

static bool small_prime(unsigned long long x) {
    if (x < 2) return false;
    for (unsigned long long d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

static unsigned long long mulm(unsigned long long a, unsigned long long b, unsigned long long k) {
    if (k < 4294967296ULL) return a * b % k;
    return (unsigned long long)((unsigned __int128)a * b % k);
}

/* number of residues mod k that are not a power y^e, e>=2 */
static unsigned long long nonpow(unsigned long long k) {
    if (k == 1) return 0;
    unsigned long long m = k, P0 = 2;
    for (unsigned long long p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned long long e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (p > P0) P0 = p;
            if (e > P0) P0 = e;
        }
    }
    if (m > 1 && m > P0) P0 = m;
    unsigned long long q = P0 + 1;
    while (!small_prime(q)) q++;
    bool isp[256];
    for (unsigned long long e = 0; e < 256; e++) isp[e] = false;
    unsigned long long lim = q < 255 ? q : 255;
    for (unsigned long long e = 2; e <= lim; e++) isp[e] = small_prime(e);
    unsigned char *mark = (unsigned char *)calloc((size_t)k, 1);
    for (unsigned long long y = 0; y < k; y++) {
        unsigned long long v = mulm(y, y, k);
        for (unsigned long long e = 2;; e++) {
            if (isp[e]) mark[v] = 1;
            if (e >= q) break;
            v = mulm(v, y, k);
        }
    }
    unsigned long long cnt = 0;
    for (unsigned long long y = 0; y < k; y++) if (!mark[y]) cnt++;
    free(mark);
    return cnt;
}

static seq_t *cand = 0;
static size_t ncand = 0, pos = 0;
static seq_t LIM = 0;
static seq_t *recs = 0;
static size_t nrecs = 0, caprecs = 0;
static unsigned long long bestnon = 0, bestk = 1;

static void regen(void) {
    if (LIM == 0) LIM = (seq_t)1 << 20; else LIM = LIM * 2;
    size_t cnt = 0;
    gen_cand((seq_t)1, 0, 200, LIM, 0, &cnt);
    if (cand) mem_free(cand);
    cand = mem_new(seq_t, cnt);
    size_t c2 = 0;
    gen_cand((seq_t)1, 0, 200, LIM, cand, &c2);
    ncand = cnt;
    /* shell sort */
    size_t gap = 1;
    while (gap < ncand / 3) gap = gap * 3 + 1;
    for (; gap > 0; gap = (gap - 1) / 3) {
        for (size_t i = gap; i < ncand; i++) {
            seq_t t = cand[i];
            size_t j = i;
            while (j >= gap && cand[j - gap] > t) { cand[j] = cand[j - gap]; j -= gap; }
            cand[j] = t;
            if (gap == 1 && false) break;
        }
        if (gap == 1) break;
    }
}

seq_t a(idx_t n) {
    while (nrecs < (size_t)n) {
        if (pos >= ncand) regen();
        seq_t c = cand[pos++];
        unsigned long long k = (unsigned long long)c;
        unsigned long long non = nonpow(k);
        bool rec = (nrecs == 0) ||
            ((unsigned __int128)non * bestk > (unsigned __int128)bestnon * k);
        if (rec) {
            if (nrecs == caprecs) {
                size_t nc = caprecs ? caprecs * 2 : 64;
                if (recs) recs = mem_resize(seq_t, recs, nc);
                else recs = mem_new(seq_t, nc);
                caprecs = nc;
            }
            recs[nrecs++] = c;
            bestnon = non;
            bestk = k;
        }
    }
    return recs[n - 1];
}
