/*
 * A350171  Add 1 to the 1st prime, then write the 2nd prime, then add 1 to
 *          the 3rd prime, then write the 4th prime, etc., alternately adding
 *          a 1 or not.
 * https://oeis.org/A350171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A350171: a(n) = prime(n) + (n is odd ? 1 : 0) */
/* offset 1 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cache of the first primes, 1‑based. */
static uint32_t *cached_primes = NULL;
static idx_t prime_cnt = 0;
static size_t prime_cap = 0;

/* Ensure that at least n primes are cached. */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;

    /* Grow storage if needed. */
    if (prime_cap < (size_t)n) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        uint32_t *tmp = (uint32_t *)realloc(cached_primes, new_cap * sizeof(uint32_t));
        if (!tmp) {
            /* Allocation failed – terminate; sequence generation cannot continue. */
            exit(EXIT_FAILURE);
        }
        cached_primes = tmp;
        prime_cap = new_cap;
    }

    /* Find next prime after the last stored one. */
    uint32_t candidate = prime_cnt ? cached_primes[prime_cnt - 1] + 1 : 2;

    while (prime_cnt < n) {
        bool is_prime = true;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = cached_primes[i];
            if ((uint64_t)p * (uint64_t)p > (uint64_t)candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            cached_primes[prime_cnt++] = candidate;
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)      /* sequence is defined for n ≥ 1 */
        return (seq_t)0;

    ensure_primes(n);

    uint32_t nth_prime = cached_primes[n - 1];   /* 1‑based indexing */
    seq_t result = (seq_t)nth_prime;
    if (n & 1)          /* odd n → add 1 */
        result += (seq_t)1;
    return result;
}
