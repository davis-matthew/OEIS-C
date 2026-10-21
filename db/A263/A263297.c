/*
 * A263297  The greater of bigomega(n) and maximal prime index in the prime
 *          factorization of n.
 * https://oeis.org/A263297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(A001222(n), A061395(n))
 * Range:     u8 n <= 1618, u16 n <= 8132, u32 n <= 8132, u64 n <= 8132,
 *            u128 n <= 8132, big any n
 * Helpers:   _smallest_prime_factor, bigomega, build_primes_upto, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) for all n >= 1, using the provided utilities */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bigomega.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/build_primes_upto.h"
#include "../utils/compute_pi.h"

static bool primes_initialized = false;
static uint64_t *prime_array = NULL;
static size_t prime_count = 0;
static idx_t  prime_limit = 0;

/* Ensure the prime array contains all primes up to 'limit' */
static void ensure_primes_limit(idx_t limit)
{
    if (!primes_initialized || limit > prime_limit) {
        uint64_t *new_tbl = NULL;
        size_t new_cnt = 0;
        build_primes_upto(limit, &new_tbl, &new_cnt);
        free(prime_array);          /* discard old table, leak is acceptable for static cache */
        prime_array = new_tbl;
        prime_count = new_cnt;
        prime_limit = limit;
        primes_initialized = true;
    }
}

/* Return the 1‑based index of a prime number 'p'.
   If the prime is not in the cached table it is computed via compute_pi. */
static seq_t prime_index(uint64_t p)
{
    if (primes_initialized && p <= prime_limit) {
        /* linear scan – tables are small for typical use (p <= 1e6) */
        for (size_t i = 0; i < prime_count; ++i) {
            if (prime_array[i] == p) return (seq_t)(i + 1);
            if (prime_array[i] > p) break;
        }
    }
    /* fallback: use the generic pi function */
    return compute_pi(p);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 0;   /* by convention */

    seq_t bigOmega = bigomega(n);   /* total number of prime factors (with multiplicity) */

    seq_t maxPrimeIdx = 0;
    uint64_t temp = (uint64_t)n;
    while (temp > 1) {
        uint64_t p = _smallest_prime_factor(temp);
        seq_t idx = prime_index(p);
        if (idx > maxPrimeIdx) maxPrimeIdx = idx;
        /* strip all copies of p */
        while (temp % p == 0) temp /= p;
    }

    return (bigOmega > maxPrimeIdx) ? bigOmega : maxPrimeIdx;
}
