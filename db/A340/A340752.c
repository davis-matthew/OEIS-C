/*
 * A340752  a(n) is the least prime p such that there is at least one prime <=
 *          p in each congruence class mod prime(n).
 * https://oeis.org/A340752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 163, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   build_primes, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes.h"

/* Computes OEIS sequence A340752.
   a(n) is the maximum of the minimal primes in each residue class
   modulo the n-th prime.  The value is the least prime p such that
   there is at least one prime <= p in every congruence class mod prime(n). */

static idx_t  *prime_list = NULL;   /* array of primes, rebuilt when needed */
static size_t  prime_count = 0;
static idx_t   prime_limit = 0;

/* Ensure that prime_list contains all primes <= limit.
   The list is rebuilt from scratch when limit increases. */
static void ensure_prime_limit(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }
    build_primes(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1)   /* offset is 1, values below are irrelevant */
        return (seq_t)0;

    /* Modulus: the n-th prime. */
    idx_t q = (idx_t)get_prime(n);
    size_t Q = (size_t)q;          /* use size_t for array indices */

    /* Allocate array for the first prime seen in each residue class. */
#if defined(__cplusplus)
    seq_t *first_prime = new seq_t[Q];
#else
    seq_t *first_prime = (seq_t *)malloc(Q * sizeof(seq_t));
    if (!first_prime) return (seq_t)0;
#endif
    for (size_t i = 0; i < Q; ++i)
        first_prime[i] = (seq_t)0;

    size_t hits = 0;          /* how many residue classes have a prime yet */

    /* Start with a modest sieve limit, grow it if needed. */
    idx_t limit = 2 * q;
    if (limit < 2)
        limit = 2;
    ensure_prime_limit(limit);

    size_t idx = 0;           /* index into prime_list */

    while (hits < Q) {
        if (idx >= prime_count) {
            /* Need more primes – double the sieve limit and rebuild. */
            limit = (limit == 0) ? 2 : limit * 2;
            ensure_prime_limit(limit);
            /* idx remains the same; new prime_count is larger. */
            continue;
        }

        idx_t pval = prime_list[idx++];
        idx_t r = pval % q;

        if (first_prime[r] == (seq_t)0) {          /* first time this residue appears */
            first_prime[r] = (seq_t)pval;
            ++hits;
        }
    }

    /* Find the maximum among the first primes for each residue. */
    seq_t maxp = (seq_t)0;
    for (size_t i = 0; i < Q; ++i)
        if (first_prime[i] > maxp)
            maxp = first_prime[i];

    /* Clean up. */
#if defined(__cplusplus)
    delete[] first_prime;
#else
    free(first_prime);
#endif

    return maxp;
}
