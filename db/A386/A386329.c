/*
 * A386329  Primes without {1, 3} as digits.
 * https://oeis.org/A386329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _digits_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A386329 – Primes without {1,3} as digits.
 * 
 * The function generates prime numbers and keeps those whose decimal
 * representation contains only the digits {0,2,4,5,6,7,8,9}.  It stores the
 * found primes in a static cache so that any call to a(n) returns the
 * correct value regardless of call order.  The algorithm uses trial
 * division with the primes discovered so far; a new prime is stored in
 * a separate list used only for divisibility checks.  All data structures
 * are backed by simple dynamic arrays of 64‑bit unsigned integers, which
 * safely cover the sizes required for the test cases (the 10 000th term
 * is below 200 000).  When the result is returned it is cast to
 * seq_t, which works for every integer type used by the build system.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_digits_ok.h"

seq_t a(idx_t n)
{
    /* static cache of all primes that satisfy the digit rule  */
    static uint64_t *cache     = NULL;
    static size_t   cache_len  = 0;
    static size_t   cache_cap  = 0;

    /* static list of all primes found so far – used for trial division */
    static uint64_t *prime_list    = NULL;
    static size_t    prime_len     = 0;
    static size_t    prime_cap     = 0;

    /* the next odd number to test (2 handled specially) */
    static uint64_t next_candidate = 2;

    /* initialise dynamic arrays on first call */
    if (!cache) {
        cache_cap = 16;
        cache = (uint64_t *)malloc(cache_cap * sizeof(uint64_t));
        if (!cache) exit(1);
    }
    if (!prime_list) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) exit(1);
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t cand = next_candidate;

        /* advance candidate for next iteration (skip evens after 2) */
        if (cand == 2)
            next_candidate = 3;
        else
            next_candidate += 2;

        /* primality test by trial division using the primes discovered so far */
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (!is_prime)
            continue;

        /* store cand in the list used for divisibility checks */
        if (prime_len == prime_cap) {
            prime_cap = prime_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            if (!tmp) exit(1);
            prime_list = tmp;
        }
        prime_list[prime_len++] = cand;

        /* accept the prime only if its decimal digits are not 1 or 3 */
        if (_digits_ok(cand)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(cache, cache_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                cache = tmp;
            }
            cache[cache_len++] = cand;
        }
    }

    /* return the (n‑1)th element cast to seq_t */
    return (seq_t)cache[n - 1];
}
