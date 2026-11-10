/*
 * A364573  Starting from k=9, each subsequent term is the next larger odd k
 *          such that A005940(k) <= k and the ratio A005940(k)/k is nearer to
 *          1.0 than for any previous k in the sequence.
 * https://oeis.org/A364573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search over odd k>=9 with A005940(k)<=k, keeping records of
 *            A005940(k)/k closest to 1. A005940(k) = product over the 1-bits
 *            of k-1, where a 1-bit's prime index is 1 + (number of 0-bits
 *            below it). Search: split k-1 into 15 low bits and a high part h,
 *            with the low bits grouped by their zero count z...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A364573_T 15
#define A364573_LOWN (1u << (A364573_T - 1))

typedef unsigned __int128 A364573_u128;

typedef struct { uint64_t v; uint32_t l; uint32_t z; } A364573_low;
typedef struct { uint64_t k; uint64_t A; } A364573_cand;

static uint64_t A364573_pr[128];
static A364573_low A364573_lo[A364573_LOWN];
static unsigned A364573_gs[A364573_T + 2];
static A364573_cand A364573_cb[A364573_LOWN];
static bool A364573_inited = false;
static uint64_t A364573_terms[512];
static size_t A364573_cnt = 0;
static uint64_t A364573_h = 0;
static uint64_t A364573_bn = 0, A364573_bd = 1;

static int A364573_cmp_low(const void *x, const void *y) {
    const A364573_low *a = (const A364573_low *)x, *b = (const A364573_low *)y;
    if (a->z != b->z) return a->z < b->z ? -1 : 1;
    if (a->v != b->v) return a->v < b->v ? -1 : 1;
    return 0;
}
static int A364573_cmp_cand(const void *x, const void *y) {
    const A364573_cand *a = (const A364573_cand *)x, *b = (const A364573_cand *)y;
    return a->k < b->k ? -1 : (a->k > b->k ? 1 : 0);
}

static void A364573_init(void) {
    int c = 0;
    for (uint64_t n = 2; c < 128; n++) {
        bool ok = true;
        for (int j = 0; j < c && A364573_pr[j] * A364573_pr[j] <= n; j++)
            if (n % A364573_pr[j] == 0) { ok = false; break; }
        if (ok) A364573_pr[c++] = n;
    }
    unsigned cnt = 0;
    for (uint32_t l = 0; l < (1u << A364573_T); l += 2) {
        uint64_t v = 1; int i = 1; uint32_t z = 0;
        for (int b = 0; b < A364573_T; b++) {
            if ((l >> b) & 1) v *= A364573_pr[i - 1];
            else { i++; z++; }
        }
        A364573_lo[cnt].v = v; A364573_lo[cnt].l = l; A364573_lo[cnt].z = z;
        cnt++;
    }
    qsort(A364573_lo, cnt, sizeof(A364573_low), A364573_cmp_low);
    for (int z = 0; z <= A364573_T + 1; z++) A364573_gs[z] = cnt;
    for (int e = (int)cnt - 1; e >= 0; e--) A364573_gs[A364573_lo[e].z] = (unsigned)e;
    for (int z = A364573_T; z >= 0; z--)
        if (A364573_gs[z] > A364573_gs[z + 1]) A364573_gs[z] = A364573_gs[z + 1];
    A364573_inited = true;
}

static void A364573_step(void) {
    const uint64_t CAP = (uint64_t)1 << 62;
    uint64_t h = A364573_h++;
    int idx[64]; int ni = 0;
    {
        uint64_t m = h; int i = 1;
        while (m > 0) {
            if (m & 1) idx[ni++] = i; else i++;
            m >>= 1;
        }
    }
    uint64_t base = h << A364573_T;
    uint64_t klo = base + 1, khi = base + (1u << A364573_T) - 1;
    size_t nc = 0;
    for (int z = 0; z <= A364573_T; z++) {
        uint64_t S = 1; bool big = false;
        for (int j = 0; j < ni; j++) {
            A364573_u128 p = (A364573_u128)S * A364573_pr[idx[j] + z - 1];
            if (p > CAP) { big = true; break; }
            S = (uint64_t)p;
        }
        if (big) continue;
        uint64_t vmax = khi / S;
        uint64_t vmin = 0;
        if (A364573_cnt > 0) {
            A364573_u128 num = (A364573_u128)A364573_bn * klo;
            A364573_u128 den = (A364573_u128)A364573_bd * S;
            vmin = (uint64_t)((num + den - 1) / den);
        }
        unsigned lo = A364573_gs[z], hi = A364573_gs[z + 1];
        while (lo < hi) {
            unsigned mid = (lo + hi) / 2;
            if (A364573_lo[mid].v < vmin) lo = mid + 1; else hi = mid;
        }
        for (unsigned e = lo; e < A364573_gs[z + 1] && A364573_lo[e].v <= vmax; e++) {
            uint64_t k = base + A364573_lo[e].l + 1;
            if (k < 9) continue;
            uint64_t A = A364573_lo[e].v * S;
            if (A <= k) { A364573_cb[nc].k = k; A364573_cb[nc].A = A; nc++; }
        }
    }
    qsort(A364573_cb, nc, sizeof(A364573_cand), A364573_cmp_cand);
    for (size_t c = 0; c < nc; c++) {
        uint64_t k = A364573_cb[c].k, A = A364573_cb[c].A;
        bool better = (A364573_cnt == 0) ||
            ((A364573_u128)A * A364573_bd > (A364573_u128)A364573_bn * k);
        if (better) {
            A364573_terms[A364573_cnt++] = k;
            A364573_bn = A; A364573_bd = k;
        }
    }
}

seq_t a(idx_t n) {
    if (!A364573_inited) A364573_init();
    while (A364573_cnt < n && A364573_cnt < 512) A364573_step();
    return (seq_t)A364573_terms[n - 1];
}
