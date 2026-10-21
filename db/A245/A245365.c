/*
 * A245365  Semiprimes of the form n*(3*n-1)/2.
 * https://oeis.org/A245365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(3*n-1)/2 Algorithm: iterate over pentagonal numbers
 *            P(k) = k*(3*k-1)/2, test each for being a product of exactly two
 *            primes (semiprime). Primes up to sqrt(P) are generated on
 *            demand; semiprimality check uses trial division by those primes.
 *            The sequence values are cached so calls for any...
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 690, u64 n <= 9258,
 *            u128 n <= 9258, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/floor_sqrt.h"

/* Global prime list (dynamic array) */
static uint64_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;
static uint64_t prime_limit = 0;

/* Ensure primes are known up to new_limit (inclusive). */
static void extend_primes(uint64_t new_limit)
{
    if (new_limit <= prime_limit) return;
    for (uint64_t cand = prime_limit + 1; cand <= new_limit; ++cand) {
        if (cand <= 1) continue;
        bool is_p = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 64;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = cand;
        }
    }
    prime_limit = new_limit;
}

/* Simple primality test for uint64_t numbers using the prime list. */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;
    for (size_t i = 1; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Test whether a uint64_t n is a product of exactly two primes. */
static bool is_semiprime_uint64(uint64_t n)
{
    if (n < 4) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) {
            uint64_t q = n / p;
            return is_prime_uint64(q);
        }
    }
    return false;
}

/* Cache for computed sequence terms */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

/* Ensure cache has space for at least 'needed' terms */
static void ensure_cache_size(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Current largest pentagonal index processed */
static idx_t current_k = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined – caller must use n >= 1 */

    /* Expand cache until it contains at least n terms */
    while (seq_len < (size_t)n) {
        ++current_k;   /* next pentagonal index k = current_k */

        /* Compute P(k) = k*(3*k-1)/2 using seq_t arithmetic */
        seq_t pent = (seq_t)current_k * ((seq_t)current_k * 3ULL - 1ULL) / 2ULL;

        /* Convert to uint64_t for semiprime test (safe up to n=10000) */
        uint64_t pent_u64 = (uint64_t)pent;

        /* Ensure primes cover up to sqrt(pent) for factorization */
        uint64_t sqrt_limit = (uint64_t)floor_sqrt(pent) + 1;
        extend_primes(sqrt_limit);

        /* Check for semiprimality */
        if (is_semiprime_uint64(pent_u64)) {
            ensure_cache_size(seq_len + 1);
            seq_cache[seq_len++] = pent;
        }
    }

    return seq_cache[n - 1];
}
