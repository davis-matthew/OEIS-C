/*
 * A351038  Numbers whose maximal digit in their primorial base expansion is
 *          not larger than the maximal exponent in their prime factorization.
 * https://oeis.org/A351038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer k such that the maximal *
 *            digit in the primorial base expansion of k is not larger than
 *            the * maximal exponent in its prime factorization. The function
 *            generates * terms by searching successive integers
 * Range:     u8 n <= 62, u16 n <= 2005, u32 n <= 9811, u64 n <= 9811,
 *            u128 n <= 9811, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes      = NULL;
static size_t   prime_cnt    = 0;
static seq_t   *cache        = NULL;
static size_t   cache_len    = 0;
static size_t   cache_cap    = 0;
static uint64_t next_candidate = 2;

/* Build a prime list large enough for the search. */
static void init_primes(void)
{
    if (prime_cnt == 0) {
        /* Plenty of primes for the first 10000 terms. */
        build_primes_upto(2000000, &primes, &prime_cnt);
    }
}

/* Maximal digit in the primorial base expansion of n (n < 2^64). */
static seq_t max_digit_primorial(uint64_t n)
{
    seq_t maxdigit = 0;
    uint64_t rem = n;
    for (size_t i = 0; i < prime_cnt && rem > 0; ++i) {
        uint64_t digit = rem % primes[i];
        if (digit > (uint64_t)maxdigit)
            maxdigit = (seq_t)digit;
        rem /= primes[i];
    }
    return maxdigit;
}

/* Maximal exponent in the prime factorisation of n (n < 2^64). */
static seq_t max_exponent(uint64_t n)
{
    seq_t maxexp = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            seq_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp > maxexp)
                maxexp = exp;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        if (maxexp == 0)
            maxexp = 1;
    }
    return maxexp;
}

/* Test whether k satisfies the defining property. */
static bool good_number(uint64_t k)
{
    return max_digit_primorial(k) <= max_exponent(k);
}

/* Main sequence routine with memoisation. */
seq_t a(idx_t n)
{
    init_primes();

    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate++;
        if (good_number(cand)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[(size_t)n - 1];
}
