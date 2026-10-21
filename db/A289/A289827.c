/*
 * A289827  a(n) = largest m <= n such that pi(m + n) = pi(m) + pi(n), where
 *          pi function is A000720 (with pi(0) = 0).
 * https://oeis.org/A289827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest m ≤ n such that π(m+n) = π(m) + π(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache for prime counts up to a certain limit.
   pi_cache[i] = π(i) for 0 ≤ i ≤ pi_limit. */
static size_t pi_limit = 0;
static uint32_t *pi_cache = NULL;
static bool *is_prime_flag = NULL;

/* Ensure that π values up to `limit` are available. */
static void ensure_pi_cache(size_t limit) {
    if (limit <= pi_limit) return;

    /* Reallocate arrays for new limit. */
    free(is_prime_flag);
    free(pi_cache);
    is_prime_flag = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_prime_flag) {
        /* If allocation fails, abort. */
        exit(EXIT_FAILURE);
    }

    /* Sieve of Eratosthenes: mark composites. */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_prime_flag[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_prime_flag[j] = true;  /* composite */
            }
        }
    }

    /* Build prefix counts of primes. */
    pi_cache = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!pi_cache) {
        exit(EXIT_FAILURE);
    }

    uint32_t count = 0;
    pi_cache[0] = 0;  /* by definition π(0) = 0 */
    for (size_t i = 1; i <= limit; ++i) {
        if (i >= 2 && !is_prime_flag[i]) {
            ++count;
        }
        pi_cache[i] = count;
    }

    pi_limit = limit;
}

/* Compute a(n).  For n >= 1, returns the largest m ≤ n such that
   π(m + n) = π(m) + π(n).  Uses the cached π values. */
seq_t a(idx_t n) {
    /* Ensure π values up to 2*n (since m ≤ n). */
    ensure_pi_cache((size_t)(2 * (size_t)n));

    seq_t pi_n = (seq_t)pi_cache[(size_t)n];

    /* Search from n down to 0. */
    for (idx_t m = n; m >= (idx_t)0; --m) {
        seq_t pi_m = (seq_t)pi_cache[(size_t)m];
        seq_t pi_mn = (seq_t)pi_cache[(size_t)(m + n)];
        if (pi_mn == pi_m + pi_n) {
            return (seq_t)m;
        }
        /* Prevent unsigned wraparound when m == 0. */
        if (m == 0) break;
    }
    /* If no m found, return 0 (happens only for n = 1). */
    return (seq_t)0;
}
