/*
 * A335792  Composite numbers k such that A335791(k) = k.
 * https://oeis.org/A335792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over composite k; with p = smallest prime factor of k and
 *            F = A335791-index of p (first i>0 with x(i) == x(2i) mod p), k
 *            is a term iff x(F) == x(2F) (mod k) and gcd(x(i) - x(2i), k) = 1
 *            for all 0 < i < F, where x(0) = 2, x(i+1) = x(i)^2 + 1.
 *            Segmented smallest-factor sieve
 * Range:     u8 n <= 18, u16 n <= 184, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9993 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t *A335792_T = NULL;
static size_t A335792_cnt = 0, A335792_cap = 0;
static uint64_t A335792_lo = 2;          /* next block start */

static uint32_t *A335792_bp = NULL;      /* base primes */
static size_t A335792_bpcnt = 0;
static uint64_t A335792_bplim = 0;       /* base primes sieved up to this */
static uint32_t *A335792_f = NULL;       /* f(p) indexed by p */
static uint64_t A335792_r = 2;

static uint64_t *A335792_buf = NULL;
static size_t A335792_bufcap = 0;

#define A335792_B 65536
static uint32_t A335792_spf[A335792_B];

static uint64_t A335792_mul(uint64_t a, uint64_t b, uint64_t m)
{
    if (m <= 0xFFFFFFFFULL)
        return (a * b) % m;
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

static uint64_t A335792_gcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static uint32_t A335792_fp(uint64_t p)
{
    uint64_t x = 2 % p, y = 2 % p;
    uint32_t i = 0;
    do {
        x = (x * x + 1) % p;
        y = (y * y + 1) % p;
        y = (y * y + 1) % p;
        ++i;
    } while (x != y);
    return i;
}

static bool A335792_ensure_base(uint64_t need)
{
    if (need <= A335792_bplim)
        return true;
    uint64_t lim = A335792_bplim * 2;
    if (lim < need) lim = need;
    if (lim < 1024) lim = 1024;
    char *c = (char *)calloc((size_t)lim + 1, 1);
    uint32_t *bp = (uint32_t *)malloc(((size_t)lim + 1) * sizeof(uint32_t));
    uint32_t *f = (uint32_t *)calloc((size_t)lim + 1, sizeof(uint32_t));
    if (!c || !bp || !f) {
        free(c); free(bp); free(f);
        return false;
    }
    size_t cnt = 0;
    for (uint64_t i = 2; i <= lim; ++i) {
        if (c[i]) continue;
        bp[cnt++] = (uint32_t)i;
        f[i] = (i < A335792_bplim && A335792_f) ? A335792_f[i] : A335792_fp(i);
        for (uint64_t j = i * i; j <= lim; j += i)
            c[j] = 1;
    }
    free(c);
    free(A335792_bp);
    free(A335792_f);
    A335792_bp = bp;
    A335792_f = f;
    A335792_bpcnt = cnt;
    A335792_bplim = lim;
    return true;
}

static bool A335792_test(uint64_t k, uint32_t F)
{
    size_t need = 2 * (size_t)F + 1;
    if (need > A335792_bufcap) {
        size_t nc = A335792_bufcap ? A335792_bufcap : 256;
        while (nc < need) nc *= 2;
        uint64_t *p = (uint64_t *)realloc(A335792_buf, nc * sizeof(uint64_t));
        if (!p) return false;
        A335792_buf = p;
        A335792_bufcap = nc;
    }
    uint64_t *x = A335792_buf;
    x[0] = 2 % k;
    for (size_t i = 1; i < need; ++i)
        x[i] = (A335792_mul(x[i - 1], x[i - 1], k) + 1) % k;
    if (x[F] != x[2 * (size_t)F])
        return false;
    uint64_t prod = 1 % k;
    for (size_t i = 1; i < F; ++i) {
        uint64_t d = x[i] >= x[2 * i] ? x[i] - x[2 * i] : x[2 * i] - x[i];
        prod = A335792_mul(prod, d, k);
        if (prod == 0)
            return false;
    }
    return A335792_gcd(prod, k) == 1;
}

static bool A335792_block(void)
{
    uint64_t lo = A335792_lo, hi = lo + A335792_B;
    while (A335792_r * A335792_r < hi)
        A335792_r++;
    if (!A335792_ensure_base(A335792_r + 1))
        return false;
    for (size_t i = 0; i < A335792_B; ++i)
        A335792_spf[i] = 0;
    for (size_t j = 0; j < A335792_bpcnt; ++j) {
        uint64_t p = A335792_bp[j];
        if (p * p >= hi) break;
        uint64_t s = ((lo + p - 1) / p) * p;
        if (s < p * p) s = p * p;
        for (uint64_t m = s; m < hi; m += p)
            if (!A335792_spf[m - lo])
                A335792_spf[m - lo] = (uint32_t)p;
    }
    for (uint64_t k = lo; k < hi; ++k) {
        uint32_t p = A335792_spf[k - lo];
        if (p == 0 || k < 4)
            continue;
        if (A335792_test(k, A335792_f[p])) {
            if (A335792_cnt == A335792_cap) {
                size_t nc = A335792_cap ? A335792_cap * 2 : 1024;
                uint64_t *q = (uint64_t *)realloc(A335792_T, nc * sizeof(uint64_t));
                if (!q) return false;
                A335792_T = q;
                A335792_cap = nc;
            }
            A335792_T[A335792_cnt++] = k;
        }
    }
    A335792_lo = hi;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    while (A335792_cnt < (size_t)n)
        if (!A335792_block())
            return 0;
    return (seq_t)A335792_T[n - 1];
}
