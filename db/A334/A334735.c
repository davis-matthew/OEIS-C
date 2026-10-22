/*
 * A334735  Denominator of Sum_{k=1..n} k^2 / Product_{k=1..n} k^2.
 * https://oeis.org/A334735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of n*(n+1)*(2*n+1)/6/(n!)^2 * * The
 *            denominator of the reduced fraction * n*(n+1)*(2*n+1)
 *            (numerator) * ──────────────────────── (denominator) * 6*(n!)² *
 *            * For each prime p ≤ n we compute * e_d = 2·v_p(n!) + v_p(6)
 *            (exponent in the raw denominator) * e_n = v_p(n) + v_p(n+1) +...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   build_primes, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes.h"   /* build_primes() */
#include "../utils/seq_mul.h"       /* seq_mul() */

/* ----- prime cache ----- */

static idx_t *prime_cache = NULL;  /* array of primes */
static size_t prime_cnt   = 0;     /* number of primes stored */
static idx_t  prime_limit = 0;     /* maximum n built so far */

/* ensure primes up to n are generated */
static void ensure_prime_cache(idx_t n)
{
    if (n <= prime_limit)
        return;

    /* free existing cache if any */
    free(prime_cache);
    prime_cache = NULL;
    prime_cnt   = 0;

    /* build new cache */
    build_primes(n, &prime_cache, &prime_cnt);
    prime_limit = n;
}

/* Legendre's formula: exponent of prime p in n! */
static idx_t legendre_exponent(idx_t n, idx_t p)
{
    idx_t exp = 0;
    while (n) {
        n /= p;
        exp += n;
    }
    return exp;
}

/* ----- main function ----- */

seq_t a(idx_t n)
{
    if (n == 0)    /* undefined, return 1 for safety */
        return (seq_t)1;

    /* Build prime cache up to n */
    ensure_prime_cache(n);

    /* start result as 1 */
    seq_t denom = (seq_t)1;

    /* iterate over primes ≤ n */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_cache[i];
        if (p > n)
            break;

        /* exponent of p in the numerator n*(n+1)*(2n+1) */
        idx_t exp_num = 0;
        idx_t tmp;

        tmp = n;
        while (tmp % p == 0) {
            ++exp_num;
            tmp /= p;
        }

        tmp = n + 1;
        while (tmp % p == 0) {
            ++exp_num;
            tmp /= p;
        }

        tmp = (idx_t)2 * n + 1;
        while (tmp % p == 0) {
            ++exp_num;
            tmp /= p;
        }

        /* exponent of p in the denominator 6*(n!)² */
        idx_t exp_den = 2 * legendre_exponent(n, p);
        if (p == 2 || p == 3)   /* v_p(6) */
            ++exp_den;

        /* remaining exponent to multiply into the result */
        if (exp_den > exp_num) {
            idx_t diff = exp_den - exp_num;
            /* multiply denom by p^diff */
            while (diff--) {
                denom = seq_mul(denom, (seq_t)p);
            }
        }
    }

    return denom;
}
