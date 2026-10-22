/*
 * A378430  a(n) = Sqrt(A378984(n)).
 * https://oeis.org/A378430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3396, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th positive integer
 * that has at least three distinct prime factors,
 * and at least two of those prime factors have exponent ≥ 2. 
 * The function enumerates integers, tests the condition
 * by trial division using a precomputed prime sieve,
 * and memoises the results. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define PRIME_LIMIT 2000000   /* enough for a(10000) and beyond */

static bool sieve_inited = false;
static uint32_t primes[PRIME_LIMIT + 1];
static size_t primes_cnt;

/* Build a prime list up to PRIME_LIMIT using a simple sieve. */
static void init_sieve(void)
{
    if (sieve_inited)
        return;
    sieve_inited = true;

    bool *is_composite = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_composite)
        exit(1);
    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        is_composite[i] = false;

    for (uint32_t p = 2; p <= PRIME_LIMIT; ++p) {
        if (!is_composite[p]) {
            primes[primes_cnt++] = p;
            if ((uint64_t)p * p <= PRIME_LIMIT) {
                for (uint32_t j = p * p; j <= PRIME_LIMIT; j += p)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
}

/* Test whether m has at least three distinct prime factors,
 * and at least two of those factors appear with exponent ≥ 2. */
static bool good_number(uint64_t m)
{
    size_t distinct = 0;
    size_t with_sq  = 0;
    uint64_t n = m;

    for (size_t i = 0; i < primes_cnt &&
            (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            ++distinct;
            unsigned exp = 0;
            do {
                n /= p;
                ++exp;
            } while (n % p == 0);
            if (exp >= 2)
                ++with_sq;
            if (distinct > 3) {
                /* we already have more than needed; but we need at least 3, so we can keep counting */
            }
        }
    }
    if (n > 1) {
        ++distinct;            /* remaining prime factor (exponent 1) */
    }
    return (distinct >= 3 && with_sq >= 2);
}

/* Cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;
static idx_t next_test = 2;   /* integer to test next */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined for offset 1, but safe */

    if (!sieve_inited)
        init_sieve();

    /* Ensure we have at least n terms cached */
    if (cache_cnt < (size_t)n) {
        if (cache == NULL) {
            cache_cap = 128;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }
        while (cache_cnt < (size_t)n) {
            if (good_number((uint64_t)next_test)) {
                if (cache_cnt == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_cnt++] = (seq_t)next_test;
            }
            ++next_test;
        }
    }

    return cache[n - 1];
}
