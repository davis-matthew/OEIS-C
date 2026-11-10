/*
 * A376865  a(1) = 1. Thereafter, if A007947(a(n-1)) is in A002110, a(n) =
 *          smallest prime not already a term (condition [A]). Otherwise, if q
 *          is the greatest prime < Gpf(a(n-1)) which does not divide a(n-1),
 *          a(n) is the least novel multiple of q,(condition [B]).
 * https://oeis.org/A376865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; if rad(a(n-1)) is a primorial (A002110), a(n)=smallest
 *            prime not yet a term; else a(n)=least novel multiple of q, q =
 *            greatest prime < gpf(a(n-1)) not dividing a(n-1). Computed by
 *            simulation
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9324 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *s376865_t = NULL;
static size_t s376865_cnt = 0, s376865_cap = 0;
static unsigned char *s376865_used = NULL;
static uint64_t s376865_ucap = 0;
static uint64_t s376865_pp = 2;

static int s376865_isprime(uint64_t x) {
    uint64_t d;
    if (x < 2) return 0;
    if (x % 2 == 0) return x == 2;
    for (d = 3; d <= x / d; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static uint64_t s376865_nextprime(uint64_t p) {
    p++;
    while (!s376865_isprime(p)) p++;
    return p;
}

static int s376865_isused(uint64_t v) {
    if (v >= s376865_ucap) return 0;
    return s376865_used[v];
}

static void s376865_mark(uint64_t v) {
    if (v >= s376865_ucap) {
        uint64_t nc = s376865_ucap ? s376865_ucap : 1024;
        unsigned char *np;
        while (nc <= v) nc *= 2;
        np = (unsigned char *)realloc(s376865_used, (size_t)nc);
        memset(np + s376865_ucap, 0, (size_t)(nc - s376865_ucap));
        s376865_used = np;
        s376865_ucap = nc;
    }
    s376865_used[v] = 1;
}

static void s376865_push(uint64_t v) {
    if (s376865_cnt == s376865_cap) {
        s376865_cap = s376865_cap ? s376865_cap * 2 : 1024;
        s376865_t = (uint64_t *)realloc(s376865_t, s376865_cap * sizeof(uint64_t));
    }
    s376865_t[s376865_cnt++] = v;
    s376865_mark(v);
}

/* returns 1 if rad(m) is a primorial; sets *gpf */
static int s376865_analyze(uint64_t m, uint64_t *gpf) {
    uint64_t expect = 2, p;
    int prim = 1;
    uint64_t g = 1;
    if (m == 1) { *gpf = 1; return 1; }
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            if (p != expect) prim = 0;
            expect = s376865_nextprime(p);
            g = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {
        if (m != expect) prim = 0;
        g = m;
    }
    *gpf = g;
    return prim;
}

seq_t a(idx_t n) {
    if (s376865_cnt == 0) {
        s376865_push(1);
    }
    while (s376865_cnt < (size_t)n) {
        uint64_t m = s376865_t[s376865_cnt - 1];
        uint64_t g, r;
        if (s376865_analyze(m, &g)) {
            while (!s376865_isprime(s376865_pp) || s376865_isused(s376865_pp))
                s376865_pp++;
            r = s376865_pp;
        } else {
            uint64_t q = g - 1, k;
            while (!(s376865_isprime(q) && m % q != 0)) q--;
            k = q;
            while (s376865_isused(k)) k += q;
            r = k;
        }
        s376865_push(r);
    }
    return (seq_t)s376865_t[(size_t)n - 1];
}
