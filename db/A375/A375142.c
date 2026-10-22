/*
 * A375142  Numbers whose powerful part (A057521) is a power of a squarefree
 *          number that is larger than 1 (A072777).
 * https://oeis.org/A375142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n‑th positive integer whose powerful part
 * (product of its prime power factors with exponent ≥2) is a power
 * of a squarefree number >1.  Equivalently, the exponents ≥2 in the
 * prime factorisation are all equal (and at least 2).  Sequence offset
 * 1 (a(1)=4).  This implementation enumerates natural numbers and
 * collects the first n such numbers. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"   /* for _floor_isqrt_u64 */

/* ------------------------------------------------------------------ */
/*  Cache of computed terms                                            */
/* ------------------------------------------------------------------ */
static seq_t *terms      = NULL;   /* array of found terms      */
static idx_t  terms_len  = 0;      /* number of cached terms    */
static idx_t  terms_cap  = 0;      /* allocated capacity        */

/* ------------------------------------------------------------------ */
/*  Prime table (uint64_t)                                             */
/* ------------------------------------------------------------------ */
static uint64_t *primes    = NULL;   /* array of primes           */
static size_t   primes_size = 0;     /* number of primes stored   */
static idx_t    prime_limit = 1;     /* largest prime generated   */

/* Ensure that primes contains all primes ≤ limit.  Prime 1 is skipped. */
static void extend_primes_to(idx_t limit)
{
    if (prime_limit >= limit) return;

    /* Allocate more space if necessary. */
    size_t new_cap = primes_size ? primes_size << 1 : 16;
    while (new_cap < limit) new_cap <<= 1;
    uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    primes = tmp;

    /* Candidate numbers start from 2. */
    for (idx_t cand = prime_limit + 1; cand <= limit; ++cand) {
        if (cand < 2) continue;
        bool is_p = true;
        for (size_t i = 0; i < primes_size; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_p = false; break; }
        }
        if (is_p) primes[primes_size++] = cand;
    }
    prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Test whether x satisfies the sequence condition.  `x` is assumed
 *  to be small enough to fit into idx_t for the sqrt test; the
 *  enumeration in this program keeps the numbers well below that.    */
/* ------------------------------------------------------------------ */
static bool is_candidate(seq_t x)
{
    seq_t temp = x;                /* working copy for division          */
    bool   has_ge2 = false;        /* at least one exponent ≥2?          */
    int    common_exp = -1;        /* common exponent value              */

    /* Limit for trial division: floor(sqrt(x)).  The value fits in uint64_t. */
    idx_t lim = (idx_t)_floor_isqrt_u64((idx_t)x);
    extend_primes_to(lim);

    /* Trial division using the prime table. */
    for (size_t i = 0; i < primes_size; ++i) {
        seq_t p_sq = (seq_t)primes[i] * (seq_t)primes[i];
        if (p_sq > temp) break;
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp >= 2) {
                if (!has_ge2) { common_exp = exp; has_ge2 = true; }
                else if (exp != common_exp) return false; /* exponents differ */
            }
        }
    }

    /* Remaining temp > 1 is a prime factor with exponent 1 – ignore. */
    return has_ge2;   /* must have at least one exponent ≥2            */
}

/* ------------------------------------------------------------------ */
/*  Ensure the terms buffer can hold at least `needed` entries.        */
/* ------------------------------------------------------------------ */
static void ensure_terms_cap(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 256;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  main sequence entry point                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* defined only for n ≥ 1 */

    static idx_t current_test = 1;            /* next integer to test */

    while (terms_len < n) {
        if (is_candidate((seq_t)current_test)) {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = (seq_t)current_test;
        }
        ++current_test;
    }
    return terms[n - 1];
}
