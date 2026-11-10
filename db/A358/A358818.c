/*
 * A358818  a(n) is the least number k such that A046660(k) = A046660(k+1) =
 *          n.
 * https://oeis.org/A358818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least k with A046660(k) = A046660(k+1) = n. Search: one
 *            of k, k+1 is odd, and that odd number j is an odd powerful
 *            number Q with excess exactly n times an odd cofactor. The code
 *            lists every such Q, walks through its odd multiples j, and tests
 *            j-1 and j+1. It doubles the bound twice (x4) pe...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A358818_pr = NULL;
static unsigned char *A358818_lg = NULL;
static size_t A358818_pc = 0;
static uint64_t A358818_plim = 0;

static void A358818_ensure_primes(uint64_t lim)
{
    if (lim <= A358818_plim) return;
    bool *comp = (bool *)calloc((size_t)lim + 1, sizeof(bool));
    if (!comp) return;
    size_t cnt = 0;
    for (uint64_t i = 2; i <= lim; ++i) {
        if (comp[i]) continue;
        ++cnt;
        for (uint64_t j = i * i; j <= lim; j += i) comp[j] = true;
    }
    uint64_t *np = (uint64_t *)malloc(sizeof(uint64_t) * (cnt ? cnt : 1));
    unsigned char *nl = (unsigned char *)malloc(cnt ? cnt : 1);
    if (!np || !nl) { free(np); free(nl); free(comp); return; }
    size_t k = 0;
    for (uint64_t i = 2; i <= lim; ++i) {
        if (comp[i]) continue;
        np[k] = i;
        unsigned char l = 0;
        uint64_t t = i;
        while (t > 1) { t >>= 1; ++l; }
        nl[k] = l; /* floor(log2 p) */
        ++k;
    }
    free(comp);
    free(A358818_pr);
    free(A358818_lg);
    A358818_pr = np;
    A358818_lg = nl;
    A358818_pc = cnt;
    A358818_plim = lim;
}

static uint64_t A358818_isqrt(uint64_t x)
{
    uint64_t r = 0, bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) { x -= r + bit; r = (r >> 1) + bit; }
        else r >>= 1;
        bit >>= 2;
    }
    return r;
}

static uint64_t A358818_icbrt(uint64_t x)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) * (r + 1) <= x) ++r;
    return r;
}

static unsigned A358818_bitlen(uint64_t x)
{
    unsigned b = 0;
    while (x) { x >>= 1; ++b; }
    return b;
}

/* true iff Omega(x) - omega(x) == target (requires primes up to cbrt(x)) */
static bool A358818_excess_is(uint64_t x, unsigned target)
{
    uint64_t r = x;
    unsigned e = 0;
    for (size_t i = 0; i < A358818_pc; ++i) {
        uint64_t p = A358818_pr[i];
        if (p * p * p > r) break;
        if (r % p == 0) {
            unsigned c = 0;
            do { r /= p; ++c; } while (r % p == 0);
            e += c - 1;
            if (e > target) return false;
        }
        unsigned need = target - e;
        if (need > 0) {
            if (r == 1) return false;
            /* Omega(r) <= bitlen(r)/floor(log2 p); need Omega(r) >= need+1 */
            unsigned bound = A358818_bitlen(r) / A358818_lg[i];
            if (bound < need + 1) return false;
        }
    }
    if (r > 1) {
        uint64_t s = A358818_isqrt(r);
        if (s * s == r) ++e;
    }
    return e == target;
}

static uint64_t A358818_X, A358818_best;
static unsigned A358818_n;

static void A358818_process(uint64_t Q)
{
    uint64_t lim = A358818_X + 1;
    unsigned n = A358818_n;
    for (uint64_t j = Q; j <= lim; j += 2 * Q) {
        if (j - 1 >= A358818_best) break;
        int ej = -1;
        if (j >= 2 && j - 1 <= A358818_X) {
            if (A358818_excess_is(j - 1, n)) {
                ej = A358818_excess_is(j, n) ? 1 : 0;
                if (ej) { A358818_best = j - 1; break; }
            }
        }
        if (j <= A358818_X && j < A358818_best && A358818_excess_is(j + 1, n)) {
            if (ej < 0) ej = A358818_excess_is(j, n) ? 1 : 0;
            if (ej) { A358818_best = j; break; }
        }
    }
}

static void A358818_enumQ(size_t idx, uint64_t cur, unsigned rem)
{
    if (rem == 0) { A358818_process(cur); return; }
    uint64_t lim = A358818_X + 1;
    for (size_t i = idx; i < A358818_pc; ++i) {
        uint64_t p = A358818_pr[i];
        uint64_t t = cur;
        bool ok = true;
        for (unsigned k = 0; k <= rem; ++k) {
            if (t > lim / p) { ok = false; break; }
            t *= p;
        }
        if (!ok) break;
        uint64_t q = cur * p * p;
        unsigned e = 2;
        for (;;) {
            A358818_enumQ(i + 1, q, rem - (e - 1));
            if (e - 1 == rem) break;
            if (q > lim / p) break;
            q *= p;
            ++e;
        }
    }
}

seq_t a(idx_t n)
{
    static uint64_t memo[64];
    static bool have[64];
    if (n >= 60) return (seq_t)0;
    if (have[n]) return (seq_t)memo[n];

    uint64_t X = 64;
    uint64_t result = 0;
    while (X <= ((uint64_t)1 << 61)) {
        uint64_t L = X + 1;
        for (idx_t i = 1; i < n; ++i) L /= 3;
        uint64_t plim = A358818_isqrt(L) + 2;
        uint64_t c = A358818_icbrt(X + 2) + 2;
        if (c > plim) plim = c;
        if (plim < 100) plim = 100;
        A358818_ensure_primes(plim);

        A358818_X = X;
        A358818_best = UINT64_MAX;
        A358818_n = (unsigned)n;
        /* odd powerful parts: primes from index 1 (skip 2) */
        A358818_enumQ(1, 1, (unsigned)n);
        if (A358818_best != UINT64_MAX) { result = A358818_best; break; }
        X *= 4;
    }
    memo[n] = result;
    have[n] = true;
    return (seq_t)result;
}
