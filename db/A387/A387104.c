/*
 * A387104  Split A386482 into maximal runs of consecutive decreasing terms;
 *          a(n) is the length of the n-th run.
 * https://oeis.org/A387104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   simulation. Generate A386482 term by term (greatest unused k <
 *            a(n-1) with gcd > 1, otherwise smallest unused k > a(n-1) with
 *            gcd > 1). For each prime p, a bitmap of used multiples of p is
 *            searched. The sequence is then split into maximal strictly
 *            decreasing runs, and a(n) = length of the n-th run
 * Range:     u8 n <= 97, u16 n <= 4434, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9784 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint64_t A387104_M = 0;
static uint32_t *A387104_spf = NULL;
static uint32_t *A387104_pr = NULL;
static size_t A387104_npr = 0;
static uint64_t *A387104_off = NULL;
static uint64_t *A387104_low = NULL;
static uint64_t *A387104_pool = NULL;

static uint64_t *A387104_runs = NULL;
static size_t A387104_rcnt = 0, A387104_rcap = 0;
static uint64_t A387104_prev = 0;
static uint64_t A387104_cur = 0;

static size_t A387104_pidx(const uint32_t *pr, size_t npr, uint32_t p)
{
    size_t lo = 0, hi = npr;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (pr[mid] < p) lo = mid + 1; else hi = mid;
    }
    return lo;
}

static uint64_t A387104_nw(uint64_t p)
{
    return (A387104_M / p) / 64 + 1;
}

/* mark value v as used in the bitmap of every prime dividing v */
static void A387104_mark(uint64_t v)
{
    uint64_t x = v;
    while (x > 1) {
        uint32_t p = A387104_spf[x];
        size_t pi = A387104_pidx(A387104_pr, A387104_npr, p);
        uint64_t *base = A387104_pool + A387104_off[pi];
        uint64_t j = v / p;
        base[j >> 6] |= (uint64_t)1 << (j & 63);
        uint64_t nw = A387104_nw(p);
        while (A387104_low[pi] < nw && base[A387104_low[pi]] == ~(uint64_t)0)
            A387104_low[pi]++;
        while (x % p == 0) x /= p;
    }
}

static void A387104_build(uint64_t M)
{
    uint64_t oldM = A387104_M;
    uint32_t *ospf = A387104_spf, *opr = A387104_pr;
    size_t onpr = A387104_npr;
    uint64_t *ooff = A387104_off, *olow = A387104_low, *opool = A387104_pool;

    A387104_M = M;
    A387104_spf = (uint32_t *)calloc((size_t)M + 1, sizeof(uint32_t));
    if (!A387104_spf) abort();
    size_t cnt = 0;
    for (uint64_t i = 2; i <= M; i++) {
        if (A387104_spf[i] == 0) {
            A387104_spf[i] = (uint32_t)i;
            cnt++;
            for (uint64_t j = i * i; j <= M; j += i)
                if (A387104_spf[j] == 0) A387104_spf[j] = (uint32_t)i;
        }
    }
    A387104_npr = cnt;
    A387104_pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    A387104_off = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    A387104_low = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!A387104_pr || !A387104_off || !A387104_low) abort();
    size_t k = 0;
    uint64_t total = 0;
    for (uint64_t i = 2; i <= M; i++) {
        if (A387104_spf[i] == i) {
            A387104_pr[k] = (uint32_t)i;
            A387104_off[k] = total;
            A387104_low[k] = 0;
            total += (M / i) / 64 + 1;
            k++;
        }
    }
    A387104_pool = (uint64_t *)calloc((size_t)total, sizeof(uint64_t));
    if (!A387104_pool) abort();
    for (size_t i = 0; i < cnt; i++)
        A387104_pool[A387104_off[i]] |= 1;  /* multiple index 0 never usable */

    if (ospf) {
        for (uint64_t x = 2; x <= oldM; x++) {
            uint32_t p = ospf[x];
            size_t pi = A387104_pidx(opr, onpr, p);
            uint64_t j = x / p;
            if ((opool[ooff[pi] + (j >> 6)] >> (j & 63)) & 1)
                A387104_mark(x);
        }
        free(ospf); free(opr); free(ooff); free(olow); free(opool);
    }
}

/* largest j in [1, j0] with multiple j unused, 0 if none */
static uint64_t A387104_down(size_t pi, uint64_t j0)
{
    if (j0 == 0) return 0;
    const uint64_t *base = A387104_pool + A387104_off[pi];
    uint64_t w = j0 >> 6;
    unsigned b = (unsigned)(j0 & 63);
    uint64_t mask = (b == 63) ? ~(uint64_t)0 : (((uint64_t)1 << (b + 1)) - 1);
    uint64_t z = ~base[w] & mask;
    for (;;) {
        if (z) return w * 64 + 63 - (uint64_t)__builtin_clzll(z);
        if (w == 0 || w <= A387104_low[pi]) return 0;
        w--;
        z = ~base[w];
    }
}

/* smallest j >= j1 with multiple j unused (may lie beyond the table) */
static uint64_t A387104_up(size_t pi, uint64_t p, uint64_t j1)
{
    uint64_t nw = A387104_nw(p);
    uint64_t w = j1 >> 6;
    if (w >= nw) return j1;
    const uint64_t *base = A387104_pool + A387104_off[pi];
    uint64_t z = ~base[w] & (~(uint64_t)0 << (j1 & 63));
    for (;;) {
        if (z) return w * 64 + (uint64_t)__builtin_ctzll(z);
        w++;
        if (w >= nw) return nw * 64;
        z = ~base[w];
    }
}

static void A387104_push_run(uint64_t len)
{
    if (A387104_rcnt >= A387104_rcap) {
        size_t nc = A387104_rcap ? A387104_rcap * 2 : 1024;
        uint64_t *t = (uint64_t *)realloc(A387104_runs, nc * sizeof(uint64_t));
        if (!t) abort();
        A387104_runs = t;
        A387104_rcap = nc;
    }
    A387104_runs[A387104_rcnt++] = len;
}

static void A387104_step(void)
{
    uint64_t prev = A387104_prev;
    uint32_t ps[16];
    int np = 0;
    uint64_t x = prev;
    while (x > 1) {
        uint32_t p = A387104_spf[x];
        ps[np++] = p;
        while (x % p == 0) x /= p;
    }
    uint64_t cand = 0;
    for (int i = 0; i < np; i++) {
        uint64_t p = ps[i];
        size_t pi = A387104_pidx(A387104_pr, A387104_npr, ps[i]);
        uint64_t j = A387104_down(pi, prev / p - 1);
        if (j && j * p > cand) cand = j * p;
    }
    if (cand == 0) {
        uint64_t best = UINT64_MAX;
        for (int i = 0; i < np; i++) {
            uint64_t p = ps[i];
            size_t pi = A387104_pidx(A387104_pr, A387104_npr, ps[i]);
            uint64_t j = A387104_up(pi, p, prev / p + 1);
            if (j * p < best) best = j * p;
        }
        cand = best;
    }
    if (cand > A387104_M) {
        uint64_t nm = A387104_M;
        while (nm < cand) nm *= 2;
        A387104_build(nm);
    }
    A387104_mark(cand);
    if (cand < prev) {
        A387104_cur++;
    } else {
        A387104_push_run(A387104_cur);
        A387104_cur = 1;
    }
    A387104_prev = cand;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A387104_spf) {
        A387104_build((uint64_t)1 << 16);
        /* a(1)=1 forms the first run; a(2)=2 starts the second */
        A387104_mark(2);
        A387104_push_run(1);
        A387104_prev = 2;
        A387104_cur = 1;
    }
    while ((uint64_t)A387104_rcnt < (uint64_t)n)
        A387104_step();
    return (seq_t)A387104_runs[(size_t)(n - 1)];
}
