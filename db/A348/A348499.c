/*
 * A348499  Numbers k such that A003968(k) is a multiple of k, where A003968
 *          is multiplicative with a(p^e) = p*(p+1)^(e-1).
 * https://oeis.org/A348499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 162, u16 n <= 9089, u32 n <= 9089, u64 n <= 9089,
 *            u128 n <= 9089, big any n
 * Helpers:   add_exponent, build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = k where k is the n‑th positive integer such that
 * A003968(k) ≡ 0 (mod k).  A003968 is multiplicative with 
 * a(p^e) = p*(p+1)^(e-1).  The test checks the exponent of each prime
 * in A003968(k) against the required exponent in k.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/int_sqrt.h"
#include "../utils/add_exponent.h"

/* ----- global prime list for factorisation ----- */
static uint64_t *prime_list = NULL;  /* array of primes */
static size_t    prime_cnt  = 0;     /* number of primes */
static idx_t     prime_limit = 0;    /* largest prime precomputed */

/* Ensure primes up to 'limit' exist.  The function leakes old memory, which
 * is acceptable for this small cache.
 */
static void
ensure_primes_upto(idx_t limit)
{
    if (prime_limit >= limit)
        return;
    if (prime_list)
        free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Factorise a 64‑bit integer 'n' into primes using the prime_list.  */
/* The caller supplies arrays big enough; max size 64 is more than enough. */
static void
factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    if (n <= 1)          /* 1 has no prime factors */
        return;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n / p)   /* p*p > n */
            break;
        if (n % p == 0) {
            size_t c = 0;
            while (n % p == 0) {
                n /= p;
                ++c;
            }
            pr[*cnt]   = p;
            exp[*cnt]  = c;
            ++(*cnt);
        }
    }
    if (n > 1) {         /* remaining prime factor */
        pr[*cnt]   = n;
        exp[*cnt]  = 1;
        ++(*cnt);
    }
}

/* Test whether k satisfies the divisibility condition. */
static bool
good_k(idx_t k)
{
    if (k == 1)
        return true;

    /* Factorise k */
    uint64_t pk[64], pe[64];
    size_t  k_cnt;
    factor_uint64((uint64_t)k, pk, pe, &k_cnt);

    /* Accumulate exponents of all primes appearing in A003968(k) */
    uint64_t exp_prime[256], exp_val[256];
    size_t   exp_cnt = 0;

    for (size_t i = 0; i < k_cnt; ++i) {
        uint64_t p = pk[i];
        uint64_t e = pe[i];

        /* the factor p itself contributes one exponent of p */
        add_exponent(exp_prime, exp_val, &exp_cnt, p, 1);

        if (e > 1) {          /* (p+1)^(e-1) contributes exponents */
            uint64_t pp = p + 1;
            /* ensure primes up to sqrt(p+1) */
            ensure_primes_upto((idx_t)int_sqrt(pp) + 1);

            uint64_t pp_factors[64], pp_exp[64];
            size_t  pp_cnt;
            factor_uint64(pp, pp_factors, pp_exp, &pp_cnt);

            uint64_t mult = e - 1;  /* multiplier for each exponent in pp */
            for (size_t j = 0; j < pp_cnt; ++j) {
                add_exponent(exp_prime, exp_val, &exp_cnt,
                               pp_factors[j], pp_exp[j] * mult);
            }
        }
    }

    /* Verify that every required exponent is satisfied */
    for (size_t i = 0; i < k_cnt; ++i) {
        uint64_t p = pk[i];
        uint64_t req = pe[i];
        uint64_t got = 0;
        for (size_t j = 0; j < exp_cnt; ++j) {
            if (exp_prime[j] == p) {
                got = exp_val[j];
                break;
            }
        }
        if (got < req)
            return false;
    }
    return true;
}

/* ----- cache of the sequence ----- */
static seq_t *terms      = NULL;  /* array of found terms */
static size_t term_len   = 0;      /* number of cached terms   */
static size_t term_cap   = 0;      /* capacity of terms array */
static idx_t  next_k     = 2;      /* next integer to test */

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)            /* the index starts at 1; guard anyway */
        return 0;

    /* Initialise cache on the first call */
    if (term_len == 0) {
        term_cap = 16;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
        if (!terms)
            return 0;
        terms[0] = (seq_t)1;
        term_len = 1;
        next_k = 2;
        /* initialise prime list for small numbers */
        ensure_primes_upto(100);   /* a small starter limit */
    }

    /* Extend cache until we have n terms */
    while (term_len < (size_t)n) {
        /* Ensure primes sufficient for factorisation of k and k+1 */
        ensure_primes_upto((idx_t)int_sqrt(next_k) + 1);
        ensure_primes_upto((idx_t)int_sqrt(next_k + 1) + 1);

        if (good_k(next_k)) {
            /* grow array if needed */
            if (term_len >= term_cap) {
                term_cap = term_cap * 2 + 16;
                seq_t *tmp = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;
                terms = tmp;
            }
            terms[term_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return terms[n - 1];
}
