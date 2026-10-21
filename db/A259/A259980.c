/*
 * A259980  Numbers that are 1+prime and have 4 proper divisors.
 * https://oeis.org/A259980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A259980: numbers that are 1+prime and have 4 proper divisors
 * (equivalently, numbers n such that n-1 is prime and n has exactly 6
 * divisors).  This implementation generates the terms on demand by
 * checking successive primes, computing the divisor count, and caching
 * the results.  The cache allows repeated calls to be answered in
 * O(1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/is_prime.h"        /* bool is_prime(idx_t x); */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt); */

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_len = 0;         /* number of cached values */
static size_t cache_cap = 0;         /* current capacity of cache */

static uint64_t *prime_list = NULL;  /* list of prime numbers for factoring */
static size_t prime_cnt = 0;         /* number of primes in prime_list */
static uint64_t prime_limit = 0;     /* largest number up to which primes are known */

/* Ensure that primes up to at least 'limit' are available.  If the
 * required limit exceeds the current one, rebuild the prime list. */
static void ensure_prime_limit(uint64_t limit) {
    if (limit <= prime_limit) return;
    uint64_t *new_list = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_list, &new_cnt);
    free(prime_list);
    prime_list = new_list;
    prime_cnt = new_cnt;
    prime_limit = limit;
}

/* Count the total number of divisors of 'n'; uses primes up to sqrt(n). */
static uint64_t divisor_count_u64(uint64_t n) {
    if (n < 2) return 1;
    uint64_t temp = n;
    uint64_t count = 1;

    /* Ensure we have primes up to sqrt(temp) */
    uint64_t sqrt_limit = (uint64_t)sqrt((long double)temp) + 1;
    ensure_prime_limit(sqrt_limit);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            count *= (exp + 1);
        }
    }
    if (temp > 1) count *= 2; /* remaining prime factor */
    return count;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1, but just in case */

    /* Grow cache until we have at least n entries */
    while (cache_len < (size_t)n) {
        /* Find next prime candidate to test.  We keep a static variable
         * holding the current integer to test for primality.  Start at 2. */
        static uint64_t candidate_prime = 2;

        /* Test whether the current integer is prime */
        if (is_prime(candidate_prime)) {
            uint64_t candidate_value = candidate_prime + 1;
            /* Count divisors (including 1 and the number itself) */
            uint64_t dcount = divisor_count_u64(candidate_value);
            if (dcount == 6) { /* exactly 4 proper divisors */
                /* Ensure cache capacity */
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE); /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)candidate_value;
            }
        }

        /* Advance candidate_prime to the next integer */
        if (candidate_prime == 2)
            candidate_prime = 3;
        else
            candidate_prime += 2; /* skip even numbers after 2 */
    }

    return cache[n - 1];
}
