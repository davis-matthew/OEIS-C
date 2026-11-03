/*
 * A308798  Primes p such that A001177(p) = (p-1)/5.
 * https://oeis.org/A308798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function generates the OEIS sequence A308798: the primes 
   p for which the multiplicative order of α = -(3+√5)/2 modulo p equals 
   (p−1)/5. The value a(n) is the n‑th such prime (a(1) = 211).
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free(seq_t *p) { mem_free(p); }

/* Representation of an element a + b√5 in GF(p)[√5] */
typedef struct { 
    seq_t a; /* coefficient of 1   */
    seq_t b; /* coefficient of √5 */
} elem_t;

static inline elem_t elem_mul(const elem_t *x, const elem_t *y, seq_t mod) {
    elem_t z;
    z.a = (x->a * y->a + (seq_t)5 * x->b * y->b) % mod;
    z.b = (x->a * y->b + x->b * y->a) % mod;
    return z;
}

static inline elem_t elem_pow(elem_t base, uint64_t exp, seq_t mod) {
    elem_t res = {1, 0};
    while (exp) {
        if (exp & 1ULL)
            res = elem_mul(&res, &base, mod);
        base = elem_mul(&base, &base, mod);
        exp >>= 1;
    }
    return res;
}

/* Test whether the prime p (given as uint64_t) satisfies the property. */
static bool prime_valid(uint64_t p_64) {
    /* p must be ≡ 1 (mod 5) – already checked by caller */
    seq_t p = (seq_t)p_64;

    uint64_t k = (p_64 - 1ULL) / 5ULL;      /* order to test */

    /* Inverse of 2 modulo p: (p+1)/2 since p is odd */
    seq_t inv2 = (p + 1) / 2;

    /* α = -(3+√5)/2 ≡ (p-3)·inv2 + (p-1)·inv2·√5  (mod p) */
    elem_t alpha;
    alpha.a = ((p - 3) * inv2) % p;
    alpha.b = ((p - 1) * inv2) % p;

    elem_t pow_k = elem_pow(alpha, k, p);
    if (pow_k.a != 1 || pow_k.b != 0)
        return false;      /* not of order k */

    /* Minimality: test with each prime divisor of k */
    uint64_t t = k;
    uint64_t orig_k = k;
    for (uint64_t d = 2; d * d <= t; ++d) {
        if (t % d == 0) {
            while (t % d == 0)
                t /= d;
            uint64_t exp = orig_k / d;
            elem_t test = elem_pow(alpha, exp, p);
            if (test.a == 1 && test.b == 0)
                return false;
        }
    }
    if (t > 1) { /* t itself is prime */
        uint64_t exp = orig_k / t;
        elem_t test = elem_pow(alpha, exp, p);
        if (test.a == 1 && test.b == 0)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Static cache for generated primes                                   */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;   /* array of primes found */
static size_t cache_len  = 0;      /* number of elements stored  */
static size_t cache_cap  = 0;      /* allocated capacity          */
static uint64_t last_checked = 1ULL; /* next candidate to test (starts at 2) */

/* Ensure the cache contains at least n primes.  */
static void generate_upto(idx_t n) {
    while (cache_len < (size_t)n) {
        ++last_checked;                     /* next integer to examine */

        if (last_checked % 5ULL != 1ULL)
            continue;                       /* only p ≡ 1 (mod 5) can work */

        if (!is_prime_big((seq_t)last_checked))
            continue;                       /* must be prime */

        if (!prime_valid(last_checked))
            continue;                       /* fails the order condition */

        /* Prime satisfies property – store it */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 8;
            seq_t *new_cache = seq_alloc_new_array(new_cap);
            if (!new_cache)
                exit(EXIT_FAILURE);          /* allocation failed */
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            if (cache)
                seq_free(cache);
            cache      = new_cache;
            cache_cap  = new_cap;
        }
        cache[cache_len++] = (seq_t)last_checked;
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                          /* safety – offset 1 is used */
    generate_upto(n);
    return cache[n - 1];
}
