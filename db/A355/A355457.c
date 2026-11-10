/*
 * A355457  Numbers k > 1 such that A354833(k) = k * A354833(k-1).
 * https://oeis.org/A355457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k>1 with A354833(k) = k*A354833(k-1); found by
 *            simulating A354833 (a(1)=1, a(k)=a(k-1)-k unless negative or
 *            already present, else a(k-1)*k) with exact bignums for the chain
 *            bases
 * Range:     u8 n <= 12, u16 n <= 17, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define A355457_L 64
#define A355457_MAXC 160

static uint32_t A355457_P[A355457_MAXC][A355457_L];
static uint64_t A355457_m[A355457_MAXC];
static uint64_t A355457_e[A355457_MAXC];
static uint64_t A355457_terms[A355457_MAXC];
static int A355457_nterms = 0;
static int A355457_nc = 0;
static int A355457_cur = 0;
static uint64_t A355457_k = 1;
static int A355457_ns = 0;
static int A355457_sc[A355457_MAXC];
static long long A355457_sd[A355457_MAXC];

static uint64_t A355457_T(uint64_t i) { return i * (i + 1) / 2; }

static int A355457_nl(const uint32_t *a)
{
    int n = A355457_L;
    while (n > 0 && a[n - 1] == 0) n--;
    return n;
}

static bool A355457_istri(uint64_t t)
{
    uint64_t v = 8 * t + 1;
    uint64_t s = (uint64_t)sqrt((double)v);
    while (s * s > v) s--;
    while ((s + 1) * (s + 1) <= v) s++;
    return s * s == v;
}

static void A355457_init(void)
{
    memset(A355457_P, 0, sizeof(A355457_P));
    A355457_P[0][0] = 1;
    A355457_m[0] = 1;
    A355457_e[0] = 1;
    A355457_nc = 1;
    A355457_cur = 0;
    A355457_k = 1;
    A355457_ns = 0;
    A355457_nterms = 0;
}

static void A355457_step(void)
{
    uint64_t k = A355457_k + 1;
    int c = A355457_cur;
    uint64_t Dk = A355457_T(k) - A355457_T(A355457_m[c]);
    uint64_t Dp = A355457_T(k - 1) - A355457_T(A355457_m[c]);
    uint32_t *P = A355457_P[c];
    bool ok = true;
    if (A355457_nl(P) <= 2) {
        uint64_t pv = (uint64_t)P[0] | ((uint64_t)P[1] << 32);
        if (pv < Dk) ok = false;
    }
    if (ok) {
        for (int s = 0; s < A355457_ns; s++) {
            int j = A355457_sc[s];
            long long x = A355457_sd[s] + (long long)Dk;
            if (x < 0) continue;
            uint64_t span = A355457_T(A355457_e[j]) - A355457_T(A355457_m[j]);
            if ((uint64_t)x > span) continue;
            if (A355457_istri(A355457_T(A355457_m[j]) + (uint64_t)x)) { ok = false; break; }
        }
    }
    if (ok) {
        A355457_e[c] = k;
        A355457_k = k;
        return;
    }
    /* multiplicative step */
    A355457_e[c] = k - 1;
    uint32_t *N = A355457_P[A355457_nc];
    memcpy(N, P, sizeof(uint32_t) * A355457_L);
    {
        uint64_t borrow = 0;
        for (int i = 0; i < A355457_L; i++) {
            uint64_t sub = borrow + (i == 0 ? (Dp & 0xFFFFFFFFu) : (i == 1 ? (Dp >> 32) : 0));
            if (sub == 0 && i > 1) break;
            uint64_t cur = N[i];
            if (cur >= sub) { N[i] = (uint32_t)(cur - sub); borrow = 0; }
            else { N[i] = (uint32_t)(cur + 0x100000000ull - sub); borrow = 1; }
        }
        uint64_t carry = 0;
        for (int i = 0; i < A355457_L; i++) {
            uint64_t v = (uint64_t)N[i] * (uint64_t)(uint32_t)k + carry;
            N[i] = (uint32_t)v;
            carry = v >> 32;
        }
    }
    A355457_m[A355457_nc] = k;
    A355457_e[A355457_nc] = k;
    /* deltas vs all earlier chains */
    A355457_ns = 0;
    for (int j = 0; j < A355457_nc; j++) {
        const uint32_t *Pj = A355457_P[j];
        int cmp = 0;
        for (int i = A355457_L - 1; i >= 0; i--) {
            if (Pj[i] != N[i]) { cmp = Pj[i] > N[i] ? 1 : -1; break; }
        }
        const uint32_t *hi = cmp >= 0 ? Pj : N;
        const uint32_t *lo = cmp >= 0 ? N : Pj;
        uint32_t d[A355457_L];
        uint64_t b = 0;
        for (int i = 0; i < A355457_L; i++) {
            uint64_t sub = (uint64_t)lo[i] + b;
            uint64_t h = hi[i];
            if (h >= sub) { d[i] = (uint32_t)(h - sub); b = 0; }
            else { d[i] = (uint32_t)(h + 0x100000000ull - sub); b = 1; }
        }
        if (A355457_nl(d) > 2) continue;
        uint64_t v = (uint64_t)d[0] | ((uint64_t)d[1] << 32);
        if (v >= (1ull << 62)) continue;
        A355457_sc[A355457_ns] = j;
        A355457_sd[A355457_ns] = cmp >= 0 ? (long long)v : -(long long)v;
        A355457_ns++;
    }
    A355457_cur = A355457_nc;
    A355457_nc++;
    A355457_k = k;
    A355457_terms[A355457_nterms++] = k;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A355457_nc == 0) A355457_init();
    while ((idx_t)A355457_nterms < n && A355457_nc < A355457_MAXC - 1)
        A355457_step();
    if ((idx_t)A355457_nterms < n) return (seq_t)0;
    return (seq_t)A355457_terms[n - 1];
}
