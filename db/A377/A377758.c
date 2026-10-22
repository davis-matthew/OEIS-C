/*
 * A377758  a(n) is the least i such that 2n-prime(i) is also a prime, where
 *          prime(i) is the i-th prime.
 * https://oeis.org/A377758
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least i such that (2*n - prime(i)) is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A377758, offset 2 (a(2) is the first term). */

#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

/* Static storage for the list of primes up to the largest requested limit. */
static uint64_t *prime_list = NULL;      /* array of primes */
static size_t   prime_count = 0;         /* number of primes stored */
static idx_t    prime_limit = 0;         /* maximum value for which primes are stored */

/* Ensure that prime_list contains all primes <= limit. */
static void ensure_primes_upto(idx_t limit) {
    if (limit <= prime_limit)
        return;                         /* already sufficient */
    /* Rebuild the prime table up to the new limit. */
    free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Main sequence function as required. */
seq_t a(idx_t n) {
    if (n < 2)
        return (seq_t)0;                 /* offset is 2; behaviour below is not important */

    /* We only need primes up to 2*n - 2, because 2*n - prime(i) must be ≥ 2. */
    idx_t limit = (idx_t)(2 * n) - 2;
    ensure_primes_upto(limit);

    idx_t twice_n = (idx_t)(2 * n);
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p   = prime_list[i];
        idx_t diff = twice_n - p;

        if (diff < 2)
            break;                       /* further primes give smaller diff, not prime */

        if (is_prime(diff))
            return (seq_t)(i + 1);       /* indices are 1-based */
    }

    /* In theory we should never reach this point, but return 0 as a safe fallback. */
    return (seq_t)0;
}
