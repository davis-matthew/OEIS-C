/*
 * A399608  Primes p such that A161351(p) is a prime.
 * https://oeis.org/A399608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 974, u32 n <= 1511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A399608: primes p such that p + sumDigits(p) + prodDigits(p)
   is prime.  The function delivers the nth term (1‑based).  It builds a list
   of primes by trial division and keeps the sequence terms in a static
   array.  Both C and C++ builds are supported; in C++ the cache is an array
   of uint64_t, while the result is cast to the arbitrary‑precision type. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"

#ifndef MAX_PRIME_CACHE
# define MAX_PRIME_CACHE 200000   /* ample space for n ≤ 10000 */
#endif

/* Static arrays holding primes found so far and the sequence terms. */
static uint64_t prime_cache[MAX_PRIME_CACHE];
static size_t prime_cache_cnt = 0;

static uint64_t term_cache[MAX_PRIME_CACHE];
static size_t term_cache_cnt = 0;

static uint64_t last_checked = 2;   /* last integer examined */

/* Helper: test whether a candidate is prime using the primes found so far. */
static bool is_prime_candidate(uint64_t cand)
{
    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > cand)          /* p is uint64_t; multiplication safe */
            break;
        if (cand % p == 0)
            return false;
    }
    return true;
}

/* Generate sequence terms until we have at least n terms. */
static void generate_terms(idx_t n)
{
    while (term_cache_cnt < n) {
        uint64_t cand = last_checked + 1;

        /* Advance cand until a prime is found. */
        while (!is_prime_candidate(cand))
            cand++;

        /* Record the prime in the cache. */
        if (prime_cache_cnt < MAX_PRIME_CACHE)
            prime_cache[prime_cache_cnt++] = cand;
        else
            return; /* cache overflow – should not happen for n ≤ 10000 */

        /* Compute q = p + sumDigits(p) + prodDigits(p) in seq_t. */
        seq_t p_seq = (seq_t)cand;
        seq_t q = p_seq + digit_sum(p_seq) + digit_product(p_seq);

        if (is_prime_big(q)) {
            /* Store the term. */
            if (term_cache_cnt < MAX_PRIME_CACHE)
                term_cache[term_cache_cnt++] = cand;
            else
                return; /* term cache overflow – should not happen */
        }

        last_checked = cand;
        last_checked++; /* move to next integer for next loop */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined for n=0, return 0 as safety */

    /* Prime 2 is a known prime but not in the sequence; initialize cache. */
    if (prime_cache_cnt == 0) {
        prime_cache[prime_cache_cnt++] = 2;
    }

    generate_terms(n);

    return (seq_t)term_cache[n - 1];
}
