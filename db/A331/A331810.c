/*
 * A331810  Smallest integer x such that prime(n) divides 1 + x + x^2 + ... +
 *          x^(q-1) where q = A023503(n), or 0 if no such x exists.
 * https://oeis.org/A331810
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 77 (2026-11-01)
 * Helpers:   factor_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = the smallest integer x > 0 such that prime(n) | (1 + x + x^2 + ... + x^(q-1)),
   where q is the largest prime factor of prime(n)-1.  
   If no such x exists, a(n) = 0.              

   The implementation uses the fact that for any prime p > 2,
   the set of solutions modulo p to x^q ≡ 1 (x ≠ 1) is a cyclic subgroup
   of order q.  We find a primitive root g of the field modulo p, then
   compute g^t (with t = (p-1)/q) once and iterate its powers to find the
   smallest non‑trivial element in that subgroup.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/powmod.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* offset: a(2) is first term */

    seq_t p = get_prime(n);                   /* large prime p   */
    uint64_t p64 = (uint64_t)p;

    /* largest prime factor of p-1  (q)  */
    uint64_t q = largest_prime_divisor(p64 - 1ULL);
    if (q <= 1) return 0;                    /* impossible, but be safe */

    uint64_t t = (p64 - 1ULL) / q;           /* (p-1)/q */

    /* factorize p-1 to get its distinct prime factors */
    size_t cnt = 0;
    uint64_t fac[64], exp[64];
    factor_u64(p64 - 1ULL, fac, exp, &cnt);  /* fac[i] are distinct primes */

    /* find a primitive root g of the field modulo p */
    seq_t g = 2;
    for (;; ++g) {
        bool ok = true;
        for (size_t i = 0; i < cnt && ok; ++i) {
            seq_t r = powmod(g, (p64 - 1ULL) / fac[i], p);
            if (r == 1) ok = false;
        }
        if (ok) break;                       /* g is a primitive root */
    }

    seq_t g_t = powmod(g, t, p);            /* g^t mod p   */

    seq_t min_val = 0;                       /* will store the answer */
    seq_t val = g_t;                         /* first non‑trivial member */
    if (min_val == 0 || val < min_val) min_val = val;

    for (uint64_t m = 2; m < q; ++m) {
        val = (val * g_t) % p;                /* next member of subgroup */
        if (val < min_val) min_val = val;
    }

    return min_val;                          /* zero only if no solution */
}
