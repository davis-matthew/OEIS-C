/*
 * A277801  a(n) = 2^(n - 1) - prime(n).
 * https://oeis.org/A277801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^(n - 1) - prime(n)
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   is_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/pow_uint.h"

/* Static storage for primes */
static uint32_t *primes = NULL;
static idx_t primes_cnt = 0;
static idx_t primes_cap = 0;

/* Ensure that the prime list contains at least n primes (prime(n) exists) */
static void _ensure_primes(idx_t n)
{
    if (primes_cnt >= n)
        return;

    /* Grow the prime storage if needed */
    if (primes == NULL) {
        primes_cap = 64;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        primes_cnt = 0;
    }

    /* Add primes until we have at least n */
    while (primes_cnt < n) {
        uint64_t candidate;
        if (primes_cnt == 0) {
            candidate = 2;
        } else {
            candidate = (uint64_t)primes[primes_cnt - 1] + 1;
            if (candidate > 2 && (candidate & 1ULL) == 0)
                candidate++; /* skip even numbers after 2 */
        }

        /* Find next prime */
        while (!is_prime((idx_t)candidate)) {
            candidate += 2; /* skip evens */
        }

        /* Grow storage if necessary */
        if (primes_cnt >= primes_cap) {
            primes_cap *= 2;
            primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
        }

        primes[primes_cnt++] = (uint32_t)candidate;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined behaviour for n < 1 */

    /* Make sure we have the n-th prime */
    _ensure_primes(n);

    seq_t prime = (seq_t)primes[n - 1];
    seq_t power = pow_uint((uint64_t)2, (idx_t)(n - 1));

    return power - prime;
}
