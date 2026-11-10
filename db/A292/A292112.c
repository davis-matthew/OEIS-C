/*
 * A292112  Records in A039654.
 * https://oeis.org/A292112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) is the n-th record value of A039654(m) for m = 2,
 *            3, 4, ..., where A039654(m) is the prime reached by iterating x
 *            -> sigma(x) - 1 from m. Values below N are factored with a
 *            growing smallest-prime-factor sieve and memoized. Larger values
 *            use trial division plus Pollard rho
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 56 (2026-11-08)
 * Helpers:   gcd_u64, is_prime_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/gcd_u64.h"

#define A292112_REC_CAP 512
#define A292112_SIEVE_CAP ((uint64_t)1 << 26)
#define A292112_BIG 0xFFFFFFFFu
#define A292112_PATH 1024

static uint64_t A292112_rec[A292112_REC_CAP];
static unsigned A292112_cnt = 0;
static uint64_t A292112_next = 2;
static uint64_t A292112_max = 0;

static uint64_t A292112_N = 0;          /* sieve covers x < N */
static uint16_t *A292112_spf = NULL;    /* odd x: smallest prime factor, 0 = prime */
static uint32_t *A292112_memo = NULL;   /* odd x: final prime, 0 = unknown */
static bool A292112_nogrow = false;

static uint32_t A292112_sp[400];
static int A292112_nsp = 0;

static void A292112_init_small_primes(void)
{
    if (A292112_nsp) return;
    for (uint32_t p = 3; p < 2048; p += 2) {
        bool pr = true;
        for (uint32_t d = 3; d * d <= p; d += 2)
            if (p % d == 0) { pr = false; break; }
        if (pr) A292112_sp[A292112_nsp++] = p;
    }
}

static void A292112_grow(uint64_t newN)
{
    uint16_t *s = (uint16_t *)calloc((size_t)(newN / 2), sizeof(uint16_t));
    if (!s) { A292112_nogrow = true; return; }
    uint32_t *mm = (uint32_t *)realloc(A292112_memo, (size_t)(newN / 2) * sizeof(uint32_t));
    if (!mm) { free(s); A292112_nogrow = true; return; }
    memset(mm + A292112_N / 2, 0, (size_t)((newN - A292112_N) / 2) * sizeof(uint32_t));
    A292112_memo = mm;
    for (uint64_t p = 3; p * p < newN; p += 2)
        if (!s[p >> 1])
            for (uint64_t q = p * p; q < newN; q += 2 * p)
                if (!s[q >> 1]) s[q >> 1] = (uint16_t)p;
    free(A292112_spf);
    A292112_spf = s;
    A292112_N = newN;
}

static void A292112_ensure(uint64_t m)
{
    if (A292112_N == 0) A292112_grow((uint64_t)1 << 16);
    while (!A292112_nogrow && A292112_N < A292112_SIEVE_CAP && (m << 3) > A292112_N)
        A292112_grow(A292112_N << 1);
}

static uint64_t A292112_sig_small(uint64_t x)
{
    uint64_t res = 1;
    if ((x & 1) == 0) {
        uint64_t pk = 1, s = 1;
        while ((x & 1) == 0) { x >>= 1; pk <<= 1; s += pk; }
        res = s;
    }
    uint32_t y = (uint32_t)x;
    while (y > 1) {
        uint32_t p = A292112_spf[y >> 1];
        if (!p) p = y;
        uint64_t pk = 1, s = 1;
        while (y % p == 0) { y /= p; pk *= p; s += pk; }
        res *= s;
    }
    return res;
}

static uint64_t A292112_f(uint64_t y, uint64_t c, uint64_t n)
{
    uint64_t r = mulmod(y, y, n) + c;
    return r >= n ? r - n : r;
}

static uint64_t A292112_rho(uint64_t n)
{
    if ((n & 1) == 0) return 2;
    for (uint64_t c = 1;; ++c) {
        uint64_t y = 2, x = 2, ys = 2, g = 1, q = 1, r = 1;
        const uint64_t M = 128;
        do {
            x = y;
            for (uint64_t i = 0; i < r; ++i) y = A292112_f(y, c, n);
            uint64_t k = 0;
            do {
                ys = y;
                uint64_t lim = (r - k < M) ? r - k : M;
                for (uint64_t i = 0; i < lim; ++i) {
                    y = A292112_f(y, c, n);
                    q = mulmod(q, x > y ? x - y : y - x, n);
                }
                g = gcd_u64(q, n);
                k += M;
            } while (k < r && g == 1);
            r <<= 1;
        } while (g == 1 && r < ((uint64_t)1 << 30));
        if (g == n || g == 0) {
            do {
                ys = A292112_f(ys, c, n);
                g = gcd_u64(x > ys ? x - ys : ys - x, n);
            } while (g == 1);
        }
        if (g != n && g != 1 && g != 0) return g;
    }
}

static void A292112_fac(uint64_t n, uint64_t *f, int *k)
{
    if (n == 1) return;
    if (is_prime_u64(n)) { f[(*k)++] = n; return; }
    uint64_t d = A292112_rho(n);
    A292112_fac(d, f, k);
    A292112_fac(n / d, f, k);
}

static uint64_t A292112_sig_big(uint64_t x)
{
    uint64_t res = 1, rem = x;
    if ((rem & 1) == 0) {
        uint64_t pk = 1, s = 1;
        while ((rem & 1) == 0) { rem >>= 1; pk <<= 1; s += pk; }
        res = s;
    }
    for (int i = 0; i < A292112_nsp; ++i) {
        uint64_t p = A292112_sp[i];
        if (p * p > rem) break;
        if (rem % p == 0) {
            uint64_t pk = 1, s = 1;
            while (rem % p == 0) { rem /= p; pk *= p; s += pk; }
            res *= s;
        }
    }
    if (rem > 1) {
        if (rem < (uint64_t)2048 * 2048 || is_prime_u64(rem)) {
            res *= rem + 1;
        } else {
            uint64_t f[64];
            int k = 0;
            A292112_fac(rem, f, &k);
            for (int i = 1; i < k; ++i) {
                uint64_t t = f[i];
                int j = i - 1;
                while (j >= 0 && f[j] > t) { f[j + 1] = f[j]; --j; }
                f[j + 1] = t;
            }
            int i = 0;
            while (i < k) {
                uint64_t p = f[i], pk = 1, s = 1;
                while (i < k && f[i] == p) { pk *= p; s += pk; ++i; }
                res *= s;
            }
        }
    }
    return res;
}

static uint64_t A292112_chain(uint64_t m)
{
    uint64_t path[A292112_PATH];
    int len = 0;
    uint64_t cur = m, res = 0;
    for (long it = 0; it < 100000; ++it) {
        if (cur < A292112_N) {
            if (cur & 1) {
                uint32_t v = A292112_memo[cur >> 1];
                if (v && v != A292112_BIG) { res = v; break; }
                if (cur > 1 && A292112_spf[cur >> 1] == 0) { res = cur; break; }
                if (len < A292112_PATH) path[len++] = cur;
            } else if (cur == 2) { res = 2; break; }
            cur = A292112_sig_small(cur) - 1;
        } else {
            if (is_prime_u64(cur)) { res = cur; break; }
            cur = A292112_sig_big(cur) - 1;
        }
    }
    if (res) {
        uint32_t st = (res < (uint64_t)A292112_BIG) ? (uint32_t)res : A292112_BIG;
        for (int i = 0; i < len; ++i) A292112_memo[path[i] >> 1] = st;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1 || n > A292112_REC_CAP) return (seq_t)0;
    A292112_init_small_primes();
    while (A292112_cnt < (unsigned)n) {
        uint64_t m = A292112_next++;
        A292112_ensure(m);
        uint64_t v = A292112_chain(m);
        if (v > A292112_max) {
            A292112_max = v;
            A292112_rec[A292112_cnt++] = v;
        }
    }
    return (seq_t)A292112_rec[n - 1];
}
