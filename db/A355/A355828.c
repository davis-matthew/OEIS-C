/*
 * A355828  Dirichlet inverse of A342671, the greatest common divisor of
 *          sigma(n) and A003961(n), where A003961 is fully multiplicative
 *          with a(p) = nextprime(p).
 * https://oeis.org/A355828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 1023, i32 n <= 9041, i64 n <= 9041,
 *            i128 n <= 9041, big any n
 * Helpers:   factor_u64, gcd, next_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A355828:  Dirichlet inverse of A342671 (the gcd of σ(n) and A003961(n)).
    Formula: a(1) = 1
             a(n) = - Σ_{d|n, d<n} A342671(n/d) * a(d)   for n > 1
    The code memoises the computed values; it works for any n ≥ 1 in any order of
    calls.  It uses helper functions from the "utils" library. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* utils that are required for the implementation */
#include "../utils/next_prime.h"      /* uint64_t next_prime(uint64_t start)      */
#include "../utils/factor_u64.h"      /* void factor_u64(uint64_t n, uint64_t *pr,
                                       uint64_t *ex, size_t *cnt) */
#include "../utils/gcd.h"             /* seq_t gcd(seq_t a, seq_t b)              */
#include "../utils/sigma_of.h"        /* seq_t sigma_of(idx_t n)                  */

/* --------------------------------------------------------------------- */
/*  integer power:  np^exp  (np up to 64‑bit, exp up to idx_t)           */
static seq_t pow_uint64(uint64_t np, idx_t exp)
{
    seq_t result = 1;
    seq_t base   = (seq_t)np;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* --------------------------------------------------------------------- */
/*  A003961(n):  product of nextprime(p)^e over prime powers p^e | n   */
static seq_t a003961_of(idx_t n)
{
    if (n <= 1u)
        return 1;

    uint64_t primes[32], exps[32];
    size_t cf = 0;
    factor_u64((uint64_t)n, primes, exps, &cf);

    seq_t prod = 1;
    for (size_t i = 0; i < cf; ++i)
    {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        uint64_t np = next_prime(p);          /* next prime after p        */
        prod *= pow_uint64(np, e);
    }
    return prod;
}

/* --------------------------------------------------------------------- */
/*  A342671(n):  gcd(σ(n), A003961(n))                                 */
static seq_t a342671_of(idx_t n)
{
    return gcd(sigma_of(n), a003961_of(n));
}

/* --------------------------------------------------------------------- */
/*  Static memoisation tables                                            */
static seq_t   *a_cache     = NULL;   /* a_cache[i] = a(i) for i ≥ 1          */
static uint8_t *a_computed = NULL;   /* 1 if a_cache[i] is valid             */
static size_t  a_cap       = 0;       /* allocated capacity (>= needed + 1)   */

/*  Ensure the tables are large enough for indices up to 'need'          */
static void ensure_cache_cap(size_t need)
{
    if (need < a_cap)
        return;
    size_t ncap = (a_cap == 0) ? 16 : a_cap << 1;
    while (ncap <= need)
        ncap <<= 1;

    /* reallocate a_cache */
    if (a_cache == NULL)
        a_cache = (seq_t *)malloc(ncap * sizeof(seq_t));
    else
        a_cache = (seq_t *)realloc(a_cache, ncap * sizeof(seq_t));

    /* reallocate a_computed */
    if (a_computed == NULL) {
        a_computed = (uint8_t *)malloc(ncap * sizeof(uint8_t));
        memset(a_computed, 0, ncap * sizeof(uint8_t));
    } else {
        a_computed = (uint8_t *)realloc(a_computed, ncap * sizeof(uint8_t));
        /* zero the newly added entries */
        memset(a_computed + a_cap, 0, (ncap - a_cap) * sizeof(uint8_t));
    }

    a_cap = ncap;
}

/* --------------------------------------------------------------------- */
/*  Recursive Dirichlet‑inverse computation (with memoisation)           */
static seq_t compute_a(idx_t n)
{
    if (n == 1u)
        return 1;

    /* bounds check and memoisation lookup */
    ensure_cache_cap((size_t)n);
    if (a_computed[(size_t)n])
        return a_cache[(size_t)n];

    /* factor n to generate all of its divisors */
    uint64_t primes[32], exps[32];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    uint64_t divs[2048];
    size_t dcnt = 1;
    divs[0] = 1;                     /* divisor 1 */

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t cur = dcnt;
        uint64_t powp = 1;
        for (uint64_t k = 1; k <= e; ++k) {
            powp *= p;
            for (size_t j = 0; j < cur; ++j)
                divs[dcnt++] = divs[j] * powp;
        }
    }

    /* sum over proper divisors */
    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        if (d >= n)            /* skip the divisor n itself */
            continue;
        seq_t bd = a342671_of(n / d);   /* A342671(n/d) */
        seq_t ad = compute_a(d);        /* a(d)       */
        sum += bd * ad;
    }

    seq_t val = -sum;
    a_cache[(size_t)n] = val;
    a_computed[(size_t)n] = 1;
    return val;
}

/* --------------------------------------------------------------------- */
/*  Public function matching the required prototype                     */
seq_t a(idx_t n)
{
    if (n < 1u)
        return 0;                 /* indices below offset are irrelevant */
    return compute_a(n);
}
