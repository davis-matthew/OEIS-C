/*
 * A333845  a(1) = a(3) = 0, and otherwise a(n) is the least multiple of
 *          prime(n) whose decimal representation ends with that of
 *          prime(n+1).
 * https://oeis.org/A333845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least positive integer that is a multiple of prime(n) and
 *            whose * decimal representation ends in the decimal
 *            representation of prime(n+1). * If no such integer exists, a(n)
 *            = 0
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   gcd_u64, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"
#include "../utils/num_digits0.h"

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

/* 64‑bit modular inverse (extended gcd).  a and m must be coprime. */
static uint64_t modinv_u64(uint64_t a, uint64_t m)
{
    int64_t t0 = 0, t1 = 1;
    int64_t r0 = m, r1 = a;
    while (r1 != 0) {
        uint64_t q = r0 / r1;
        int64_t  r = r0 - q * r1;
        int64_t  t = t0 - q * t1;
        r0 = r1;  r1 = r;
        t0 = t1;  t1 = t;
    }
    /* r0 holds gcd(a,m).  It must be 1.  Normalize t0 to [0,m). */
    if (t0 < 0) t0 += m;
    return (uint64_t)t0;
}

/* 64‑bit greatest common divisor. */

/* Sequence function. */
seq_t a(idx_t n)
{
    /* Prime(n) and Prime(n+1).  get_prime returns a uint32_t. */
    uint64_t p = (uint64_t)get_prime(n);
    uint64_t q = (uint64_t)get_prime(n + 1);

    /* Base 10 modulus: 10^d, where d = number of decimal digits of q. */
    uint64_t d = num_digits0(q);
    uint64_t mod = 1;
    for (uint64_t i = 0; i < d; ++i) mod *= 10ULL;   /* mod <= 10^6 for the value range */

    /* Solve p * k ≡ q (mod mod). */
    uint64_t g = gcd_u64(p, mod);
    if (q % g != 0) return (seq_t)0;    /* no solution → a(n)=0 */

    uint64_t p1 = p / g;
    uint64_t m1 = mod / g;
    uint64_t q1 = q / g;

    uint64_t inv = modinv_u64(p1 % m1, m1);
    uint64_t k   = (q1 % m1) * inv % m1;   /* smallest positive k */

    seq_t result = (seq_t)p * (seq_t)k;
    return result;
}
