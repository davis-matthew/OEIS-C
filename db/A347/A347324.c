/*
 * A347324  Row sums in A347738 when that sequence is written as a triangle.
 * https://oeis.org/A347324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = sum of row n of A347738 (row n = one inventory pass).
 *            Computed by simulating the passes. cnt[v] is how many terms
 *            equal v so far. G is how many terms are >= the current threshold
 *            t. Each step appends c = G, adds 1 to G when c >= t, then
 *            subtracts cnt[t] from G and moves to t+1. A row ends...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define A347324_SMALL 65536u
#define A347324_MAXROWS 64

static uint32_t *A347324_small = NULL;   /* counts for v < SMALL */
static uint8_t  *A347324_big = NULL;     /* saturating counts for v >= SMALL */
static size_t    A347324_big_cap = 0;
static uint64_t *A347324_hk = NULL;      /* overflow hash: key */
static uint32_t *A347324_hv = NULL;      /* overflow hash: extra count */
static size_t    A347324_hcap = 0, A347324_hcnt = 0;

static uint64_t A347324_total = 0;
static idx_t    A347324_rows = 0;
static uint64_t A347324_sums[A347324_MAXROWS];

static size_t A347324_hslot(const uint64_t *keys, size_t cap, uint64_t k)
{
    size_t i = (size_t)((k * 0x9E3779B97F4A7C15ULL) >> 20) & (cap - 1);
    while (keys[i] != 0 && keys[i] != k) i = (i + 1) & (cap - 1);
    return i;
}

static void A347324_hgrow(void)
{
    size_t nc = A347324_hcap ? A347324_hcap * 2 : 1024;
    uint64_t *nk = (uint64_t*)calloc(nc, sizeof(uint64_t));
    uint32_t *nv = (uint32_t*)calloc(nc, sizeof(uint32_t));
    for (size_t i = 0; i < A347324_hcap; ++i) {
        if (A347324_hk[i]) {
            size_t j = A347324_hslot(nk, nc, A347324_hk[i]);
            nk[j] = A347324_hk[i];
            nv[j] = A347324_hv[i];
        }
    }
    free(A347324_hk); free(A347324_hv);
    A347324_hk = nk; A347324_hv = nv; A347324_hcap = nc;
}

static uint64_t A347324_hget(uint64_t k)
{
    if (!A347324_hcap) return 0;
    size_t i = A347324_hslot(A347324_hk, A347324_hcap, k);
    return A347324_hk[i] ? A347324_hv[i] : 0;
}

static void A347324_hinc(uint64_t k)
{
    if (2 * (A347324_hcnt + 1) > A347324_hcap) A347324_hgrow();
    size_t i = A347324_hslot(A347324_hk, A347324_hcap, k);
    if (!A347324_hk[i]) { A347324_hk[i] = k; A347324_hv[i] = 0; ++A347324_hcnt; }
    A347324_hv[i]++;
}

static void A347324_ensure(uint64_t v)
{
    if (v < A347324_big_cap) return;
    size_t nc = A347324_big_cap ? A347324_big_cap : 1u << 20;
    while (nc <= v) nc += nc / 2;
    uint8_t *nb = (uint8_t*)realloc(A347324_big, nc);
    memset(nb + A347324_big_cap, 0, nc - A347324_big_cap);
    A347324_big = nb;
    A347324_big_cap = nc;
}

static uint64_t A347324_get(uint64_t v)
{
    if (v < A347324_SMALL) return A347324_small[v];
    if (v >= A347324_big_cap) return 0;
    uint8_t b = A347324_big[v];
    if (b < 255) return b;
    return 255 + A347324_hget(v);
}

static void A347324_inc(uint64_t v)
{
    if (v < A347324_SMALL) { A347324_small[v]++; return; }
    A347324_ensure(v);
    if (A347324_big[v] < 255) A347324_big[v]++;
    else A347324_hinc(v);
}

seq_t a(idx_t n)
{
    if (!A347324_small)
        A347324_small = (uint32_t*)calloc(A347324_SMALL, sizeof(uint32_t));
    while (A347324_rows <= n && A347324_rows < A347324_MAXROWS) {
        uint64_t G = A347324_total, t = 0, sum = 0;
        for (;;) {
            uint64_t c = G;
            A347324_inc(c);
            A347324_total++;
            if (c == 0) break;
            sum += c;
            if (c >= t) G++;
            G -= A347324_get(t);
            t++;
        }
        A347324_sums[A347324_rows] = sum;
        A347324_rows++;
    }
    return (seq_t)A347324_sums[n];
}
