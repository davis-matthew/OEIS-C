/*
 * A307718  Primes p such that a + b is prime and a^2 + b^2 = p^2 and p = c +
 *          d such that c^2 + d^2 = e^2.
 * https://oeis.org/A307718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   _floor_isqrt_u64, gcd, is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A307718 – Primes p such that
 *  (i)  a² + b² = p² is a primitive Pythagorean triple and a + b is prime,
 *  (ii) p = c + d where (c,d,e) is a primitive Pythagorean triple.   */
/*  The term a(n) is the nth prime satisfying the above conditions.  */
/*  Implementation uses Euclid’s parametrisation of primitive triples
 *  and a simple cache so that each term after the first is found in
 *  O(1) after it has been generated. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/*  Utility functions supplied by the build  */
#include "../utils/gcd.h"              /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t n); */
#include "../utils/next_prime.h"       /* uint64_t next_prime(uint64_t start); */
#include "../utils/_floor_isqrt_u64.h" /* idx_t _floor_isqrt_u64(idx_t n); */

/*  Integer square root for uint64_t values.  */
static inline uint64_t isqrt_u64(uint64_t n)
{
    return (uint64_t)_floor_isqrt_u64(n);
}

/*  (i)  Does prime p satisfy the first condition?  */
static bool first_condition(uint64_t p)
{
    /* Pythagorean hypotenuse must be 1 (mod 4). */
    if ((p & 3) != 1)
        return false;

    uint64_t limit = isqrt_u64(p / 2);        /* n ≤ sqrt(p/2) to keep m > n. */
    for (uint64_t n = 1; n <= limit; ++n) {
        uint64_t rem = p - n * n;
        uint64_t m   = isqrt_u64(rem);
        if (m * m != rem)
            continue;                        /* not a perfect square. */
        if (m <= n)
            continue;
        if (gcd((seq_t)m, (seq_t)n) != 1)
            continue;
        if (((m ^ n) & 1) == 0)
            continue;                        /* parity must differ for primitiveness */

        /* Compute legs of the triple */
        uint64_t a = m * m - n * n;
        uint64_t b = 2 * m * n;
        uint64_t sum = a + b;
        if (is_prime_small((seq_t)sum))
            return true;
    }
    return false;
}

/*  (ii)  Does prime p satisfy the second condition?  */
static bool second_condition(uint64_t p)
{
    uint64_t limit = isqrt_u64(p / 2 + 1);     /* v ≤ sqrt(p/2+1) to keep u > v. */
    for (uint64_t v = 1; v <= limit; ++v) {
        __uint128_t t128 = (__uint128_t)p + 2ULL * v * v;
        if (t128 > UINT64_MAX)                 /* overflow – stop early. */
            break;
        uint64_t t = (uint64_t)t128;
        uint64_t s = isqrt_u64(t);
        if (s * s != t)
            continue;
        if (s <= v)
            continue;
        uint64_t u = s - v;
        if (gcd((seq_t)u, (seq_t)v) != 1)
            continue;
        if (((u ^ v) & 1) == 0)
            continue;                        /* parity must differ for primitiveness */

        return true; /* u,v form a primitive triple whose legs sum to p. */
    }
    return false;
}

/*  ---------------- Cache handling ----------------  */
static seq_t *cache = NULL;   /* Array of discovered primes. */
static size_t clen  = 0;      /* Current number of cached terms. */
static size_t ccap  = 0;      /* Allocated capacity of cache. */

/*  Ensure the cache holds at least new_cap elements.  */
static void ensure_cache_cap(size_t new_cap)
{
    if (new_cap <= ccap)
        return;

    size_t newsize = ccap ? (ccap << 1) : 16;
    while (newsize < new_cap)
        newsize <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp)
        abort();                 /* Unsuitable for judge, should not happen. */

    cache = tmp;
    ccap  = newsize;
}

/*  Generate terms until the cache contains at least target entries.  */
static void generate_terms(size_t target)
{
    if (clen >= target)
        return;

    uint64_t candidate = clen ? (uint64_t)cache[clen - 1] : 0;

    while (clen < target) {
        candidate = next_prime(candidate);  /* next prime after current candidate */
        if (candidate <= 2)
            continue;                       /* 2 cannot be expressed as m²+n². */
        if (!first_condition(candidate))
            continue;                       /* fails hypotenuse condition */
        if (!second_condition(candidate))
            continue;                       /* fails sum‑of‑legs condition */

        ensure_cache_cap(clen + 1);
        cache[clen++] = (seq_t)candidate;
    }
}

/*  ---------------- Required function ----------------  */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 – undefined for n=0, but safe. */
        return 0;

    size_t target = (size_t)n;
    generate_terms(target);
    return cache[n - 1];
}
