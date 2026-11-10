/*
 * A336470  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A336466(i) = A336466(j) and A336158(i) = A336158(j), for all i,
 *          j >= 1.
 * https://oeis.org/A336470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the ordered pair
 *            [A336466(n), A336158(n)], where A336466 is fully multiplicative
 *            with A336466(p) = A000265(p-1) and A336158(n) =
 *            A046523(A000265(n)). Computed by scanning k = 1..n and numbering
 *            each distinct pair the first time it appears
 * Range:     u8 n <= 988, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9637 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static const uint64_t A336470_sp[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};

/* Both A336466(k) and A336158(k) are <= k, so they fit in uint64_t. */
static void A336470_pair(uint64_t k, uint64_t *x, uint64_t *y)
{
    uint64_t v1 = 1;
    uint64_t exps[20];
    int cnt = 0;
    uint64_t m = k;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            uint64_t o = p - 1;
            while (o > 0 && (o & 1) == 0) o >>= 1;
            for (int i = 0; i < e; ++i) v1 *= o;
            if (p != 2) exps[cnt++] = (uint64_t)e;
        }
    }
    if (m > 1) {
        uint64_t o = m - 1;
        while (o > 0 && (o & 1) == 0) o >>= 1;
        v1 *= o;
        if (m != 2) exps[cnt++] = 1;
    }
    for (int i = 0; i < cnt; ++i)
        for (int j = i + 1; j < cnt; ++j)
            if (exps[i] < exps[j]) { uint64_t t = exps[i]; exps[i] = exps[j]; exps[j] = t; }
    uint64_t v2 = 1;
    for (int i = 0; i < cnt; ++i)
        for (uint64_t e = 0; e < exps[i]; ++e)
            v2 *= A336470_sp[i];
    *x = v1;
    *y = v2;
}

/* Hash table of pairs -> class id */
static uint64_t *A336470_hx = NULL;
static uint64_t *A336470_hy = NULL;
static uint64_t *A336470_hv = NULL;   /* 0 = empty, else id */
static size_t A336470_hcap = 0, A336470_hcnt = 0;

static uint64_t *A336470_res = NULL;
static idx_t A336470_done = 0, A336470_rcap = 0;

static size_t A336470_hash(uint64_t x, uint64_t y, size_t cap)
{
    uint64_t h = x * 0x9E3779B97F4A7C15ULL ^ (y + 0x632BE59BD9B4E019ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    return (size_t)(h & (uint64_t)(cap - 1));
}

static void A336470_hinsert_raw(uint64_t x, uint64_t y, uint64_t v)
{
    size_t i = A336470_hash(x, y, A336470_hcap);
    while (A336470_hv[i] != 0) i = (i + 1) & (A336470_hcap - 1);
    A336470_hx[i] = x; A336470_hy[i] = y; A336470_hv[i] = v;
}

static void A336470_hgrow(void)
{
    size_t oc = A336470_hcap;
    uint64_t *ox = A336470_hx, *oy = A336470_hy, *ov = A336470_hv;
    size_t nc = oc ? oc * 2 : 4096;
    A336470_hx = (uint64_t *)calloc(nc, sizeof(uint64_t));
    A336470_hy = (uint64_t *)calloc(nc, sizeof(uint64_t));
    A336470_hv = (uint64_t *)calloc(nc, sizeof(uint64_t));
    A336470_hcap = nc;
    for (size_t i = 0; i < oc; ++i)
        if (ov[i] != 0) A336470_hinsert_raw(ox[i], oy[i], ov[i]);
    free(ox); free(oy); free(ov);
}

static uint64_t A336470_lookup(uint64_t x, uint64_t y)
{
    if (A336470_hcap == 0 || (A336470_hcnt + 1) * 2 > A336470_hcap) A336470_hgrow();
    size_t i = A336470_hash(x, y, A336470_hcap);
    while (A336470_hv[i] != 0) {
        if (A336470_hx[i] == x && A336470_hy[i] == y) return A336470_hv[i];
        i = (i + 1) & (A336470_hcap - 1);
    }
    A336470_hcnt++;
    A336470_hx[i] = x; A336470_hy[i] = y; A336470_hv[i] = (uint64_t)A336470_hcnt;
    return (uint64_t)A336470_hcnt;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n > A336470_done) {
        if (n > A336470_rcap) {
            idx_t nc = A336470_rcap ? A336470_rcap : 1024;
            while (nc < n) nc *= 2;
            uint64_t *nr = (uint64_t *)calloc((size_t)(nc + 1), sizeof(uint64_t));
            if (A336470_res != NULL) {
                for (idx_t i = 0; i <= A336470_done; ++i) nr[i] = A336470_res[i];
                free(A336470_res);
            }
            A336470_res = nr;
            A336470_rcap = nc;
        }
        for (idx_t k = A336470_done + 1; k <= n; ++k) {
            uint64_t x, y;
            A336470_pair((uint64_t)k, &x, &y);
            A336470_res[k] = A336470_lookup(x, y);
        }
        A336470_done = n;
    }
    return (seq_t)A336470_res[n];
}
