/*
 * A378884  Numbers that are not powers of primes and whose two smallest prime
 *          divisors are consecutive primes.
 * https://oeis.org/A378884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   smallest_prime_factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A378884 – numbers that are not powers of primes and whose two
   smallest prime divisors are consecutive primes.
   Implementation: brute‐force search with trial division for primality
   and smallest prime factor.  This works for the values used in the
   test harness (n ≤ 10000). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/smallest_prime_factor_u64.h"

/* ---------------------------------------------------------------- */
/* Small helper routines                                              */
/* ---------------------------------------------------------------- */

/* Test whether a number is prime (trial division). */
static inline bool is_prime_u64(uint64_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* Return the next prime after start (start ≥ 2). */
static inline uint64_t next_prime_u64(uint64_t start) {
    uint64_t p = start + 1;
    while (!is_prime_u64(p)) ++p;
    return p;
}

/* ---------------------------------------------------------------- */
/* Test whether a candidate number is a term of the sequence.      */
/* ---------------------------------------------------------------- */

static inline bool is_valid_candidate(uint64_t m) {
    if (m <= 5) return false;             /* first term is 6 */
    uint64_t p1 = smallest_prime_factor_u64(m);
    if (p1 == m) return false;            /* prime or prime power */

    /* remove all factors of p1 */
    uint64_t r = m;
    while (r % p1 == 0) r /= p1;
    if (r == 1) return false;             /* m = p1^k */

    uint64_t p2 = smallest_prime_factor_u64(r);
    uint64_t next_p1 = next_prime_u64(p1);
    return p2 == next_p1;
}

/* ---------------------------------------------------------------- */
/* Sequence storage and retrieval                                   */
/* ---------------------------------------------------------------- */

static seq_t *term_cache = NULL;
static size_t cache_sz = 0;
static size_t cache_cap = 0;
static uint64_t last_candidate = 5;        /* next to try after 5 */

/* Ensure there is space for one more element */
static void grow_cache(void) {
    if (cache_cap == 0) {
        cache_cap = 16;
        term_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        assert(term_cache != NULL);
    } else if (cache_sz >= cache_cap) {
        cache_cap *= 2;
        term_cache = (seq_t *)realloc(term_cache, cache_cap * sizeof(seq_t));
        assert(term_cache != NULL);
    }
}

/* ---------------------------------------------------------------- */
/* Desired function.  It may be called in any order.                 */
/* ---------------------------------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset = 1, but undefined elsewhere */

    /* generate terms until we have the nth term */
    while (cache_sz < (size_t)n) {
        uint64_t candidate = last_candidate + 1;
        last_candidate = candidate;

        if (is_valid_candidate(candidate)) {
            grow_cache();
            term_cache[cache_sz++] = (seq_t)candidate;
        }

        /* safety: avoid infinite loop if candidate overflows */
        if (candidate == UINT64_MAX) {
            abort();
        }
    }

    return term_cache[(size_t)n - 1];
}
