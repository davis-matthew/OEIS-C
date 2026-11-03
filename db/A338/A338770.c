/*
 * A338770  a(n) is the sum of primes of the form n - 2*p for primes p < n/2.
 * https://oeis.org/A338770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 1574, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9214 (2026-11-01)
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = Σ q  where n = 2*p + q, p prime, p < n/2, and q prime */
/* The function is memoized: an internal sieve is expanded only when a larger n is requested. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes.h"

/* Helper: binary search to test membership of x in an array of primes */
static bool prime_in_list(idx_t x, idx_t *primes, size_t cnt)
{
    if (x < 2) return false;
    size_t lo = 0;
    size_t hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == x) return true;
        if (primes[mid] < x)   lo = mid + 1;
        else                   hi = mid;
    }
    return false;
}

/* External entry point for the sequence */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Persistent sieve state */
    static idx_t sieve_limit = 0;
    static idx_t *prime_list = NULL;
    static size_t prime_count = 0;

    /* Expand the sieve if necessary */
    if (n > sieve_limit) {
        free(prime_list);                      /* safe if prime_list is NULL */
        build_primes(n, &prime_list, &prime_count);
        sieve_limit = n;
    }

    seq_t sum = (seq_t)0;
    idx_t half = n / 2;

    for (size_t i = 0; i < prime_count && prime_list[i] < half; ++i) {
        idx_t p = prime_list[i];
        idx_t q = n - (idx_t)2 * p;
        if (q >= 2 && prime_in_list(q, prime_list, prime_count))
            sum += (seq_t)q;
    }

    return sum;
}
