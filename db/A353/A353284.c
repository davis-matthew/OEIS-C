/*
 * A353284  Consider the number of divisors tau(k) of every composite k
 *          between prime p >= 3 and the next prime; if the largest tau(k) is
 *          a prime power, then p is in the sequence.
 * https://oeis.org/A353284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that the largest number of divisors
 *            of composite integers between p and the next prime is a prime
 *            power
 * Range:     u8 n <= 27, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   divisor_count_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"  /* divisor_count_u64 */
#include "../utils/next_prime.h"        /* next_prime */

static inline bool is_prime_power_uint64(uint64_t x)
{
    if (x < 2) return false;
    uint64_t rem = x;
    uint64_t pf = 0;
    for (uint64_t d = 2; d * d <= rem; ++d) {
        if (rem % d == 0) {
            if (pf != 0 && d != pf) return false;
            pf = d;
            while (rem % d == 0) rem /= d;
        }
    }
    if (rem > 1) {
        if (pf == 0) return true;      /* rem itself is prime */
        if (rem != pf) return false;
    }
    return true;
}

static seq_t *term_cache = NULL;
static size_t term_cap   = 0;
static size_t term_cnt   = 0;
static uint64_t last_prime = 2;  /* start before the first prime in sequence */

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms calculated. */
    while (term_cnt < (size_t)n) {
        /* Find next prime after the last processed prime. */
        uint64_t p = next_prime(last_prime);
        if (p < 3) { /* should not happen, but protect against weird API */
            last_prime = p;
            continue;
        }

        uint64_t q = next_prime(p);               /* next prime after p */
        uint64_t max_tau = 0;

        for (uint64_t k = p + 1; k < q; ++k) {
            uint64_t tau = divisor_count_u64(k);
            if (tau < 3) continue;                /* skip primes (tau=2) */
            if (tau > max_tau) max_tau = tau;
        }

        if (max_tau > 0 && is_prime_power_uint64(max_tau)) {
            /* Store the prime p in the cache. */
            if (term_cnt == term_cap) {
                size_t new_cap = term_cap ? term_cap * 2 : 8;
                term_cache = (seq_t *)realloc(term_cache,
                                               new_cap * sizeof(seq_t));
                term_cap = new_cap;
            }
            term_cache[term_cnt++] = (seq_t)p;
        }

        last_prime = p;   /* prepare for next iteration */
    }

    return term_cache[(size_t)n - 1];
}
