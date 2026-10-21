/*
 * A263922  Highest exponent in prime factorization of n-th central binomial
 *          coefficient.
 * https://oeis.org/A263922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = highest exponent in the prime factorization of the
 *            central binomial coefficient C(2n,n) (OEIS A263922). The
 *            exponent of a prime p in C(2n,n) is computed via Legendre's
 *            formula and the maximum over all primes ≤ 2n is returned
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static idx_t max_prime_limit = 0;      /* highest prime limit prepared (≤ 2*max_n seen) */
static uint64_t *prime_list = NULL;    /* primes ≤ max_prime_limit                */
static size_t prime_cnt = 0;           /* number of primes in prime_list           */

/* Ensure we have a list of all primes up to 'limit'. Rebuild if needed. */
static void
ensure_primes(idx_t limit)
{
    if (!prime_list || limit > max_prime_limit) {
        free(prime_list);                           /* discard old list */
        build_primes_upto(limit, &prime_list, &prime_cnt);
        max_prime_limit = limit;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset is 1; n=0 not used but safe */

    /* Prepare primes up to 2*n */
    ensure_primes(2 * n);

    idx_t two_n = 2 * n;
    idx_t n_seq = n;
    idx_t max_exp = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        idx_t power = p;
        idx_t exp = 0;

        /* Legendre's formula for exponent of p in C(2n,n) */
        while (power <= two_n) {
            exp += (two_n / power) - 2 * (n_seq / power);
            /* prevent overflow of 'power' */
            if (power > two_n / p) break;
            power *= p;
        }
        if (exp > max_exp) max_exp = exp;
    }

    return (seq_t)max_exp;
}
