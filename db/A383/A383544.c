/*
 * A383544  Numbers k such that A383543(k) > A383543(m) for all m < k.
 * https://oeis.org/A383544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(n) is the n-th k where A383543(k)
 *            sets a new record. Suppose the current record value is r. A new
 *            record needs i*k+1 squarefree for 1 <= i <= r+1. - For every
 *            prime p with p^2 <= r+2, k must then be a multiple of p, so k
 *            only steps through multiples of the product P o...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define A383544_PLIM 2700000u
#define A383544_TABMAX 251u

static uint32_t *A383544_pr = NULL;
static size_t A383544_np = 0;

static void A383544_init_primes(void)
{
    if (A383544_pr) return;
    unsigned char *comp = (unsigned char *)calloc(A383544_PLIM + 1, 1);
    size_t cap = 210000;
    A383544_pr = (uint32_t *)malloc(cap * sizeof(uint32_t));
    for (uint32_t i = 2; i <= A383544_PLIM; ++i) {
        if (!comp[i]) {
            if (A383544_np == cap) {
                cap *= 2;
                A383544_pr = (uint32_t *)realloc(A383544_pr, cap * sizeof(uint32_t));
            }
            A383544_pr[A383544_np++] = i;
            if ((uint64_t)i * i <= A383544_PLIM)
                for (uint32_t j = i * i; j <= A383544_PLIM; j += i) comp[j] = 1;
        }
    }
    free(comp);
}

static uint64_t A383544_isqrt(uint64_t x)
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

/* inverse of a modulo m, gcd(a,m)=1, 0<a<m, m < 2^62 */
static int64_t A383544_inv(int64_t a, int64_t m)
{
    int64_t old_r = a, r = m, old_s = 1, s = 0;
    while (r != 0) {
        int64_t q = old_r / r, t;
        t = old_r - q * r; old_r = r; r = t;
        t = old_s - q * s; old_s = s; s = t;
    }
    old_s %= m;
    if (old_s < 0) old_s += m;
    return old_s;
}

/* Exact test: returns true iff A383543(k) > r, storing A383543(k) in *fout. */
static bool A383544_check(uint64_t k, uint64_t r, uint64_t *fout)
{
    uint64_t m = UINT64_MAX;
    bool broke = false;
    for (size_t j = 0; j < A383544_np; ++j) {
        uint64_t p = A383544_pr[j];
        uint64_t M = p * p;
        if (m != UINT64_MAX && (M - 1) / k >= m) { broke = true; break; }
        if (k % p == 0) continue;
        uint64_t aa = k % M;
        uint64_t iv = (uint64_t)A383544_inv((int64_t)aa, (int64_t)M);
        uint64_t i0 = M - iv;
        if (i0 <= r + 1) return false;
        if (i0 < m) m = i0;
    }
    uint64_t f = m - 1;
    if (!broke) {
        uint64_t L = A383544_pr[A383544_np - 1];
        uint64_t L2 = L * L;
        for (uint64_t i = 1; i < m; ++i) {
            uint64_t v = i * k + 1;
            uint64_t qmax = v / L2;
            bool bad = false;
            for (uint64_t q = 1; q <= qmax; ++q) {
                if (v % q == 0) {
                    uint64_t w = v / q, s = A383544_isqrt(w);
                    if (s * s == w) { bad = true; break; }
                }
            }
            if (bad) {
                if (i <= r + 1) return false;
                f = i - 1;
                break;
            }
        }
    }
    *fout = f;
    return true;
}

static unsigned char *A383544_tab = NULL;
static uint64_t A383544_tmod[64];
static size_t A383544_toff[64];
static size_t A383544_nt = 0;

static void A383544_build_tables(uint64_t r)
{
    size_t total = 0;
    A383544_nt = 0;
    for (size_t j = 0; j < A383544_np && A383544_pr[j] <= A383544_TABMAX; ++j) {
        uint64_t p = A383544_pr[j], M = p * p;
        if (M <= r + 2) continue;
        A383544_tmod[A383544_nt] = M;
        A383544_toff[A383544_nt] = total;
        total += (size_t)M;
        A383544_nt++;
    }
    free(A383544_tab);
    A383544_tab = (unsigned char *)calloc(total + 1, 1);
    for (size_t t = 0; t < A383544_nt; ++t) {
        uint64_t M = A383544_tmod[t];
        uint64_t p = A383544_isqrt(M);
        unsigned char *b = A383544_tab + A383544_toff[t];
        for (uint64_t i = 1; i <= r + 1; ++i) {
            if (i % p == 0) continue;
            uint64_t iv = (uint64_t)A383544_inv((int64_t)(i % M), (int64_t)M);
            b[(M - iv) % M] = 1;
        }
    }
}

seq_t a(idx_t n)
{
    static uint64_t recs[256];
    static uint64_t cnt = 0;
    static uint64_t lastr = 0;
    A383544_init_primes();
    if (cnt == 0) {
        uint64_t f = 0;
        A383544_check(1, 0, &f);
        recs[0] = 1;
        lastr = f;
        cnt = 1;
    }
    while (cnt < n && cnt < 256) {
        uint64_t P = 1;
        for (size_t j = 0; j < A383544_np; ++j) {
            uint64_t p = A383544_pr[j];
            if (p * p <= lastr + 2) P *= p;
            else break;
        }
        A383544_build_tables(lastr);
        uint64_t k = (recs[cnt - 1] / P + 1) * P;
        for (;; k += P) {
            bool pass = true;
            for (size_t t = 0; t < A383544_nt; ++t) {
                if (A383544_tab[A383544_toff[t] + (size_t)(k % A383544_tmod[t])]) {
                    pass = false;
                    break;
                }
            }
            if (!pass) continue;
            uint64_t f;
            if (A383544_check(k, lastr, &f)) {
                lastr = f;
                recs[cnt++] = k;
                break;
            }
        }
    }
    return (seq_t)recs[n - 1];
}
