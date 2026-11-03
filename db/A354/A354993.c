/*
 * A354993  a(n) = 1 if A344005(d) < A344005(n) for all the proper divisors d
 *          of n, otherwise 0. Here A344005(n) is the smallest positive
 *          integer m such that n divides m*(m+1).
 * https://oeis.org/A354993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A344005(d) < A344005(n) for all proper divisors d of
 *            n, otherwise 0. A344005(n) is the smallest m>0 such that n
 *            divides m*(m+1). The implementation caches A344005 values up to
 *            the largest n requested
 * Range:     u8 n <= 1875, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 10000 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/*--------------------- Helper: extended gcd and modular inverse ---------------------*/
static int64_t egcd_int64(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
    if (b == 0) { *x = 1; *y = 0; return a; }
    int64_t x1, y1;
    int64_t g = egcd_int64(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

/*--------------------- Factorization into prime powers ---------------------*/
static void factor_into_prime_powers(uint64_t n, uint64_t *prime, uint64_t *pow, int *cnt)
{
    *cnt = 0;
    uint64_t t = n;
    if (t % 2 == 0) {
        uint64_t pe = 1;
        while (t % 2 == 0) { t /= 2; pe *= 2; }
        prime[*cnt] = 2;
        pow[*cnt] = pe;
        (*cnt)++;
    }
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            uint64_t pe = 1;
            while (t % p == 0) { t /= p; pe *= p; }
            prime[*cnt] = p;
            pow[*cnt] = pe;
            (*cnt)++;
        }
    }
    if (t > 1) {                /* remaining prime factor */
        prime[*cnt] = t;
        pow[*cnt] = t;
        (*cnt)++;
    }
}

static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
{
    int64_t x, y;
    int64_t g = egcd_int64((int64_t)a, (int64_t)mod, &x, &y);
    if (g != 1) return 0ULL;                    /* not invertible – shouldn't happen */
    int64_t res = x % (int64_t)mod;
    if (res < 0) res += (int64_t)mod;
    return (uint64_t)res;
}

/*--------------------- Compute A344005(n) ---------------------*/
static seq_t A344005(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t primes[8];
    uint64_t powes[8];
    int k;
    factor_into_prime_powers((uint64_t)n, primes, powes, &k);

    uint64_t best = (uint64_t)-1;
    int total = 1 << k;         /* 2^k combinations of remainders */

    for (int mask = 0; mask < total; ++mask) {
        uint64_t res = 0;         /* current solution modulo cur */
        uint64_t cur = 1;         /* product of processed moduli */
        for (int i = 0; i < k; ++i) {
            uint64_t mi = powes[i];
            uint64_t ri = ((mask >> i) & 1) ? (mi - 1) : 0;
            uint64_t x = (ri - (res % mi) + mi) % mi;
            uint64_t inv = modinv_uint64(cur % mi, mi);
            uint64_t t = (x * inv) % mi;
            res += cur * t;
            cur *= mi;
        }
        uint64_t m_candidate = (res == 0) ? (uint64_t)n : res;
        if (m_candidate < best) best = m_candidate;
    }
    return (seq_t)best;
}

/*--------------------- Simple cache for A344005 values ---------------------*/
static seq_t *cacheA = NULL;   /* 1‑indexed array of cached values */
static idx_t cacheSize = 0;    /* highest index currently cached */

/* Ensure the cache contains indices up to 'upto' (inclusive). */
static void ensureCache(idx_t upto)
{
    if (upto <= cacheSize) return;
    idx_t newSize = upto + 1;                    /* include index 0, which we ignore */
    seq_t *tmp = mem_resize(seq_t, cacheA, newSize);
    if (tmp == NULL) exit(1);                    /* out of memory */
    if (cacheSize == 0) {
        for (idx_t i = 0; i <= upto; ++i) tmp[i] = 0;
    } else {
        for (idx_t i = cacheSize + 1; i <= upto; ++i) tmp[i] = 0;
    }
    cacheA = tmp;
    cacheSize = upto;
}

/*--------------------- Main sequence function ---------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* unused, but guard anyway */

    ensureCache(n);                              /* make sure indices 1..n exist */

    if (cacheA[n] == 0) cacheA[n] = A344005(n);
    seq_t mn = cacheA[n];

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d, d2 = n / d;
            if (d1 != n) {
                if (cacheA[d1] == 0) cacheA[d1] = A344005(d1);
                if (cacheA[d1] >= mn) return (seq_t)0;
            }
            if (d2 != d1 && d2 != n) {
                if (cacheA[d2] == 0) cacheA[d2] = A344005(d2);
                if (cacheA[d2] >= mn) return (seq_t)0;
            }
        }
    }
    return (seq_t)1;
}
