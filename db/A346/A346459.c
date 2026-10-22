/*
 * A346459  Triangle read by rows: T(n,k) = 0 if all positive integers can be
 *          colored with two colors without any positive integer x being the
 *          same color as n*x or k*x; otherwise, T(n,k) = 1 (for 2 <= k <= n).
 * https://oeis.org/A346459
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 iff there exist positive integers x,y such that r^x =
 *            k^y and x + y is odd, where (r,k) is the row and column of the
 *            triangle read by rows from index n (offset 2). The function
 *            determines the minimal base representation of r and k by prime
 *            factorisation and then applies the parity criterion
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses trial‑division factorisation; the running
   time is O(√max(r,k)) which is well below the limits for the
   required range of n. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Greatest common divisor for uint64_t. */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Factorises n into distinct prime factors in primes[] and the
   corresponding exponents in exps[] (arrays of at least 64 elements).
   *cnt is set to the number of distinct prime factors. */
static void factor_u64(uint64_t n, uint64_t primes[], uint64_t exps[],
                      size_t *cnt)
{
    *cnt = 0;
    if (n <= 1) return;

    /* factor 2 */
    if ((n & 1ULL) == 0) {
        uint64_t e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        primes[*cnt] = 2ULL;
        exps[*cnt]   = e;
        ++(*cnt);
    }

    /* odd factors */
    for (uint64_t p = 3ULL; p <= n / p; p += 2ULL) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[*cnt] = p;
            exps[*cnt]   = e;
            ++(*cnt);
        }
    }

    if (n > 1) {                     /* remaining prime */
        primes[*cnt] = n;
        exps[*cnt]   = 1;
        ++(*cnt);
    }
}

/* Converts the prime factorisation of n to the minimal base
   representation:  n = a^g  where g is the gcd of the exponents.
   primes[] and exp_div[] receive the base primes and the exponents
   in the base a (i.e. exps[i]/g).  *cnt becomes the number of
   distinct base primes, *gExp is g. */
static void minimal_base(uint64_t n, uint64_t primes[], uint64_t exp_div[],
                         size_t *cnt, uint64_t *gExp)
{
    uint64_t fp[64];
    uint64_t fe[64];
    size_t fcnt;
    factor_u64(n, fp, fe, &fcnt);

    uint64_t g = fe[0];
    for (size_t i = 1; i < fcnt; ++i)
        g = gcd_u64(g, fe[i]);

    *gExp = g;
    *cnt  = fcnt;
    for (size_t i = 0; i < fcnt; ++i) {
        primes[i]   = fp[i];
        exp_div[i] = fe[i] / g;
    }
}

/* Returns true if the minimal base representations of r and k are equal. */
static bool same_minimal_base(uint64_t r, uint64_t k)
{
    uint64_t pr[64], pk[64];
    uint64_t er[64], ek[64];
    size_t cr, ck;
    uint64_t gR, gK;

    minimal_base(r, pr, er, &cr, &gR);
    minimal_base(k, pk, ek, &ck, &gK);

    if (cr != ck) return false;
    for (size_t i = 0; i < cr; ++i) {
        if (pr[i] != pk[i] || er[i] != ek[i]) return false;
    }
    return true;
}

/* Computes T(r,k) as described: 1 iff r and k share a minimal base
   and (gR + gK)/gcd(gR,gK) is odd.  r >= k >= 2. */
static bool triangle_entry(uint64_t r, uint64_t k)
{
    if (!same_minimal_base(r, k))
        return false;           /* different bases */

    /* Recover the exponent gR of r and gK of k */
    uint64_t pr[64], pk[64];
    uint64_t er[64], ek[64];   /* exponents in the base; not used further */
    size_t cr, ck;
    uint64_t gR, gK;

    minimal_base(r, pr, er, &cr, &gR);
    minimal_base(k, pk, ek, &ck, &gK);

    uint64_t d = gcd_u64(gR, gK);
    uint64_t s = (gR / d) + (gK / d);   /* (gR + gK) / d */

    return (s & 1ULL) != 0;
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined for n < 2 */

    idx_t idx = n - 2;            /* zero‑based index within the triangle */

    /* Find row r such that C(r-1) <= idx < C(r), where C(r)=r*(r-1)/2 */
    idx_t r = 2;
    while ((unsigned __int128)r * (r - 1) / 2 <= idx) ++r;

    /* Column k within that row (k starts at 2) */
    idx_t prev   = (unsigned __int128)(r - 1) * (r - 2) / 2;
    idx_t offset = idx - prev;
    idx_t k      = 2 + offset;

    bool val = triangle_entry((uint64_t)r, (uint64_t)k);
    return val ? (seq_t)1 : (seq_t)0;
}
