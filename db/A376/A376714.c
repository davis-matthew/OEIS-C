/*
 * A376714  Sum of squares of the decimal digits of the n-th prime.
 * https://oeis.org/A376714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of squares of decimal digits of the n-th prime
 * Range:     u8 n <= 668, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Compute the n-th term of OEIS A376714 */
seq_t a(idx_t n)
{
    /* OEIS offset is 1, a(1) corresponds to the first prime (2) */
    if (n == 0)
        return 0;                     /* undefined for n<1, return 0 just in case */

    /* Static cache of the primes that have already been found.  */
    static uint64_t *primes = NULL;     /* array of primes */
    static size_t count = 0;            /* number of primes stored */
    static size_t cap   = 0;            /* current allocated capacity */

    /* Grow the cache until it contains at least n primes. */
    while (count < n) {
        uint64_t cand = (count == 0) ? 2 : primes[count - 1] + 1;

        /* Find the next prime after the last stored one.  */
        while (!is_prime(cand))
            ++cand;

        /* Ensure enough space for the new entry. */
        if (count >= cap) {
            size_t new_cap = (cap == 0) ? 8 : cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            if (!tmp) abort();          /* memory allocation failed */
            primes = tmp;
            cap = new_cap;
        }

        primes[count++] = cand;
    }

    /* Compute the digit‑square sum of the n‑th prime. */
    uint64_t p = primes[n - 1];
    uint64_t sum = 0;
    while (p) {
        uint64_t d = p % 10;
        sum += d * d;
        p /= 10;
    }

    return (seq_t)sum;   /* cast to the build’s seq_t type */
}
