/*
 * A262464  Indices of rows of triangle A262432 where the maximum term of the
 *          row is a new record.
 * https://oeis.org/A262464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not a closed form. Sieve: work out sigma(x) for x = 1, 2, 3, ...
 *            in segments, set den(x) = x/gcd(sigma(x), x) (A017666), and
 *            count how often each denominator occurs. The largest count goes
 *            up by exactly 1 at each new record row, and a(n) is the x where
 *            it first reaches n. The scan is incremental...
 * Range:     u8 n <= 7, u16 n <= 37, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Validated: standard, pass, 62 terms through n = 125 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define A262464_SEG 65536

static uint64_t *A262464_recs = NULL;
static uint64_t  A262464_rcnt = 0, A262464_rcap = 0;
static uint8_t  *A262464_cnt = NULL;
static uint64_t  A262464_ccap = 0;
static uint64_t  A262464_done = 0;      /* all x <= done processed */
static uint64_t  A262464_best = 0;
static uint32_t *A262464_primes = NULL;
static size_t    A262464_np = 0;
static uint64_t  A262464_plim = 0;

static uint64_t A262464_seg_rem[A262464_SEG];
static uint64_t A262464_seg_sig[A262464_SEG];

static uint64_t A262464_bgcd(uint64_t u, uint64_t v)
{
    if (u == 0) return v;
    if (v == 0) return u;
    int s = __builtin_ctzll(u | v);
    u >>= __builtin_ctzll(u);
    do {
        v >>= __builtin_ctzll(v);
        if (u > v) { uint64_t t = u; u = v; v = t; }
        v -= u;
    } while (v);
    return u << s;
}

static void A262464_build_primes(uint64_t lim)
{
    uint8_t *comp = (uint8_t *)calloc((size_t)(lim + 1), 1);
    if (!comp) exit(1);
    free(A262464_primes);
    A262464_np = 0;
    size_t cap = 1024;
    A262464_primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!A262464_primes) exit(1);
    for (uint64_t i = 2; i <= lim; ++i) {
        if (comp[i]) continue;
        if (A262464_np == cap) {
            cap *= 2;
            A262464_primes = (uint32_t *)realloc(A262464_primes, cap * sizeof(uint32_t));
            if (!A262464_primes) exit(1);
        }
        A262464_primes[A262464_np++] = (uint32_t)i;
        for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
    }
    free(comp);
    A262464_plim = lim;
}

static void A262464_push(uint64_t x)
{
    if (A262464_rcnt == A262464_rcap) {
        A262464_rcap = A262464_rcap ? A262464_rcap * 2 : 256;
        A262464_recs = (uint64_t *)realloc(A262464_recs, (size_t)A262464_rcap * sizeof(uint64_t));
        if (!A262464_recs) exit(1);
    }
    A262464_recs[A262464_rcnt++] = x;
}

static void A262464_segment(void)
{
    uint64_t lo = A262464_done + 1;
    uint64_t hi = lo + A262464_SEG;          /* exclusive */
    size_t len = (size_t)(hi - lo);

    /* primes up to sqrt(hi) */
    if (A262464_plim * A262464_plim < hi) {
        uint64_t nl = A262464_plim ? A262464_plim * 2 : 1024;
        while (nl * nl < hi) nl *= 2;
        A262464_build_primes(nl);
    }

    /* counts array must cover denominators up to hi (den(x) <= x) */
    if (A262464_ccap < hi) {
        uint64_t nc = A262464_ccap ? A262464_ccap * 2 : (1u << 20);
        while (nc < hi) nc *= 2;
        A262464_cnt = (uint8_t *)realloc(A262464_cnt, (size_t)nc);
        if (!A262464_cnt) exit(1);
        memset(A262464_cnt + A262464_ccap, 0, (size_t)(nc - A262464_ccap));
        A262464_ccap = nc;
    }

    for (size_t i = 0; i < len; ++i) {
        A262464_seg_rem[i] = lo + i;
        A262464_seg_sig[i] = 1;
    }
    for (size_t pi = 0; pi < A262464_np; ++pi) {
        uint64_t p = A262464_primes[pi];
        if (p * p >= hi) break;
        uint64_t start = ((lo + p - 1) / p) * p;
        for (uint64_t m = start; m < hi; m += p) {
            size_t i = (size_t)(m - lo);
            uint64_t r = A262464_seg_rem[i] / p;
            uint64_t pk = p, s = 1 + p;
            while (r % p == 0) { r /= p; pk *= p; s += pk; }
            A262464_seg_rem[i] = r;
            A262464_seg_sig[i] *= s;
        }
    }
    for (size_t i = 0; i < len; ++i) {
        uint64_t x = lo + i;
        uint64_t sg = A262464_seg_sig[i];
        if (A262464_seg_rem[i] > 1) sg *= A262464_seg_rem[i] + 1;
        uint64_t g = A262464_bgcd(sg, x);
        uint64_t den = x / g;
        uint8_t c = ++A262464_cnt[den];
        if (c > A262464_best) {
            A262464_best = c;
            A262464_push(x);
        }
    }
    A262464_done = hi - 1;
}

seq_t a(idx_t n)
{
    while (A262464_rcnt < (uint64_t)n)
        A262464_segment();
    return (seq_t)A262464_recs[n - 1];
}
