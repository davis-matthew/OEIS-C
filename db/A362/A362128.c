/*
 * A362128  Indices of records in A360179.
 * https://oeis.org/A362128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th record in A360179: simulate A360179
 *            (a(1) = 1; if a(n-1) is novel then a(n) = d(a(n-1)), else a(n) =
 *            a(n-1) + d(u) with u the smallest unstarred prior term, which is
 *            then starred) and record each index where a new maximum appears.
 *            This is a simulation, not a closed formula
 * Range:     u8 n <= 18, u16 n <= 190, u32 n <= 8238, u64 n <= 8238,
 *            u128 n <= 8238, big any n
 * Validated: standard, pass, 75 terms through n = 7901 (2026-11-10)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/isqrt_u64.h"

#define A362128_B 65536u

static uint64_t *A362128_seenw = NULL;  /* bitmap: value already occurred      */
static uint64_t *A362128_bitw  = NULL;  /* bitmap: cnt[v] > 0                  */
static uint8_t  *A362128_cntb  = NULL;  /* unstarred multiplicity (<=255)      */
static uint8_t  *A362128_dcb   = NULL;  /* divisor counts, 255 = "large"       */
static uint8_t  *A362128_dfill = NULL;  /* per block: divisor counts sieved    */
static size_t    A362128_cap   = 0;     /* value capacity, multiple of B       */
static size_t    A362128_loword = 0;    /* bitmap words below this are empty   */

static uint64_t *A362128_heap = NULL;   /* overflow / below-pointer min-heap   */
static size_t    A362128_hn = 0, A362128_hcap = 0;

static uint32_t *A362128_pr = NULL;     /* primes up to A362128_plim           */
static size_t    A362128_np = 0;
static uint64_t  A362128_plim = 0;

static uint64_t *A362128_recs = NULL;
static size_t    A362128_rec_n = 0, A362128_rec_cap = 0;

static uint64_t A362128_i = 0, A362128_prev = 0, A362128_prev2 = 0, A362128_max = 0;
static bool     A362128_prev_novel = true;

static void A362128_ensure(uint64_t v)
{
    if (v < A362128_cap) return;
    size_t nc = A362128_cap ? A362128_cap + A362128_cap / 2 : ((size_t)1 << 20);
    while (nc <= v) nc += nc / 2;
    nc = (nc + A362128_B - 1) / A362128_B * A362128_B;
    size_t ow = A362128_cap / 64, nw = nc / 64;
    size_t ob = A362128_cap / A362128_B, nb = nc / A362128_B;
    A362128_seenw = (uint64_t *)realloc(A362128_seenw, nw * sizeof(uint64_t));
    A362128_bitw  = (uint64_t *)realloc(A362128_bitw,  nw * sizeof(uint64_t));
    A362128_cntb  = (uint8_t  *)realloc(A362128_cntb,  nc);
    A362128_dcb   = (uint8_t  *)realloc(A362128_dcb,   nc);
    A362128_dfill = (uint8_t  *)realloc(A362128_dfill, nb);
    memset(A362128_seenw + ow, 0, (nw - ow) * sizeof(uint64_t));
    memset(A362128_bitw  + ow, 0, (nw - ow) * sizeof(uint64_t));
    memset(A362128_cntb + A362128_cap, 0, nc - A362128_cap);
    memset(A362128_dcb  + A362128_cap, 0, nc - A362128_cap);
    memset(A362128_dfill + ob, 0, nb - ob);
    A362128_cap = nc;
}

static void A362128_ensure_primes(uint64_t lim)
{
    if (lim <= A362128_plim) return;
    uint64_t L = A362128_plim * 2;
    if (L < lim) L = lim;
    if (L < 1024) L = 1024;
    uint8_t *comp = (uint8_t *)calloc((size_t)L + 1, 1);
    free(A362128_pr);
    A362128_pr = (uint32_t *)malloc(((size_t)L / 2 + 2) * sizeof(uint32_t));
    A362128_np = 0;
    for (uint64_t p = 2; p <= L; ++p) {
        if (comp[p]) continue;
        A362128_pr[A362128_np++] = (uint32_t)p;
        for (uint64_t q = p * p; q <= L; q += p) comp[q] = 1;
    }
    free(comp);
    A362128_plim = L;
}

static void A362128_fill(size_t blk)
{
    static uint64_t rem[A362128_B];
    static uint32_t dd[A362128_B];
    uint64_t lo = (uint64_t)blk * A362128_B, hi = lo + A362128_B;
    A362128_ensure_primes(isqrt_u64(hi) + 1);
    for (uint32_t k = 0; k < A362128_B; ++k) { rem[k] = lo + k; dd[k] = 1; }
    for (size_t t = 0; t < A362128_np; ++t) {
        uint64_t p = A362128_pr[t];
        if (p * p >= hi) break;
        uint64_t s = (lo + p - 1) / p * p;
        if (s == 0) s = p;
        for (uint64_t j = s; j < hi; j += p) {
            uint32_t k = (uint32_t)(j - lo);
            uint32_t e = 0;
            do { rem[k] /= p; ++e; } while (rem[k] % p == 0);
            dd[k] *= e + 1;
        }
    }
    for (uint32_t k = 0; k < A362128_B; ++k) {
        uint32_t d = dd[k];
        if (rem[k] > 1) d *= 2;
        A362128_dcb[lo + k] = (uint8_t)(d >= 255 ? 255 : d);
    }
    A362128_dfill[blk] = 1;
}

static uint32_t A362128_d(uint64_t v)
{
    A362128_ensure(v);
    size_t blk = (size_t)(v / A362128_B);
    if (!A362128_dfill[blk]) A362128_fill(blk);
    uint32_t r = A362128_dcb[v];
    if (r < 255) return r;
    A362128_ensure_primes(isqrt_u64(v) + 1);
    uint64_t m = v;
    r = 1;
    for (size_t t = 0; t < A362128_np; ++t) {
        uint64_t p = A362128_pr[t];
        if (p * p > m) break;
        if (m % p == 0) {
            uint32_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            r *= e + 1;
        }
    }
    if (m > 1) r *= 2;
    return r;
}

static void A362128_hpush(uint64_t v)
{
    if (A362128_hn == A362128_hcap) {
        A362128_hcap = A362128_hcap ? A362128_hcap * 2 : 1024;
        A362128_heap = (uint64_t *)realloc(A362128_heap, A362128_hcap * sizeof(uint64_t));
    }
    size_t i = A362128_hn++;
    while (i > 0) {
        size_t p = (i - 1) / 2;
        if (A362128_heap[p] <= v) break;
        A362128_heap[i] = A362128_heap[p];
        i = p;
    }
    A362128_heap[i] = v;
}

static uint64_t A362128_hpop(void)
{
    uint64_t top = A362128_heap[0];
    uint64_t last = A362128_heap[--A362128_hn];
    size_t i = 0, n = A362128_hn;
    for (;;) {
        size_t c = 2 * i + 1;
        if (c >= n) break;
        if (c + 1 < n && A362128_heap[c + 1] < A362128_heap[c]) ++c;
        if (A362128_heap[c] >= last) break;
        A362128_heap[i] = A362128_heap[c];
        i = c;
    }
    if (n) A362128_heap[i] = last;
    return top;
}

static void A362128_push(uint64_t v)
{
    A362128_ensure(v);
    if ((size_t)(v >> 6) < A362128_loword || A362128_cntb[v] == 255) {
        A362128_hpush(v);
        return;
    }
    if (A362128_cntb[v]++ == 0)
        A362128_bitw[v >> 6] |= (uint64_t)1 << (v & 63);
}

static uint64_t A362128_pop(void)
{
    size_t w = A362128_loword, nw = A362128_cap / 64;
    while (w < nw && A362128_bitw[w] == 0) ++w;
    A362128_loword = w;
    uint64_t m1 = UINT64_MAX;
    unsigned b = 0;
    if (w < nw) {
        b = (unsigned)__builtin_ctzll(A362128_bitw[w]);
        m1 = (uint64_t)w * 64 + b;
    }
    if (A362128_hn && A362128_heap[0] < m1) return A362128_hpop();
    if (--A362128_cntb[m1] == 0)
        A362128_bitw[w] &= ~((uint64_t)1 << b);
    return m1;
}

static void A362128_add_rec(uint64_t ix)
{
    if (A362128_rec_n == A362128_rec_cap) {
        A362128_rec_cap = A362128_rec_cap ? A362128_rec_cap * 2 : 256;
        A362128_recs = (uint64_t *)realloc(A362128_recs, A362128_rec_cap * sizeof(uint64_t));
    }
    A362128_recs[A362128_rec_n++] = ix;
}

seq_t a(idx_t n)
{
    if (A362128_i == 0) {
        A362128_ensure(1);
        A362128_seenw[0] |= (uint64_t)1 << 1;
        A362128_prev = 1;
        A362128_prev2 = 0;
        A362128_prev_novel = true;
        A362128_max = 1;
        A362128_i = 1;
        A362128_add_rec(1);
    }
    while (A362128_rec_n < (size_t)n) {
        uint64_t i = A362128_i;
        if (i >= 2) A362128_push(A362128_prev2);
        uint64_t nxt;
        if (A362128_prev_novel) {
            nxt = A362128_d(A362128_prev);
        } else {
            uint64_t u = A362128_pop();
            nxt = A362128_prev + A362128_d(u);
        }
        A362128_ensure(nxt);
        uint64_t bit = (uint64_t)1 << (nxt & 63);
        bool isnew = (A362128_seenw[nxt >> 6] & bit) == 0;
        A362128_seenw[nxt >> 6] |= bit;
        A362128_prev2 = A362128_prev;
        A362128_prev = nxt;
        A362128_prev_novel = isnew;
        A362128_i = i + 1;
        if (nxt > A362128_max) {
            A362128_max = nxt;
            A362128_add_rec(i + 1);
        }
    }
    return (seq_t)A362128_recs[n - 1];
}
