/*
 * A377972  a(n) is the greatest i such that 2n-prime(i) is also a prime,
 *          where prime(i) is the i-th prime.
 * https://oeis.org/A377972
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest i such that 2*n - prime(i) is also prime (offset
 *            2)
 * Range:     u8 n <= 810, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* undefined for n < 2, but return 0 to avoid UB */
    if (n < 2)
        return (seq_t)0;

    /* 2*n fits in idx_t (which is unsigned 64‑bit) */
    idx_t limit = n * ((idx_t)2);

    /* static cache of primes up to the current limit */
    static uint64_t *prime_list   = NULL; /* array of primes */
    static size_t    prime_count  = 0;    /* number of primes in array */
    static idx_t     sieve_limit  = 0;    /* highest limit already sieved */

    /* if we need a larger sieve, free the old array and rebuild */
    if (limit > sieve_limit) {
        if (prime_list) {
            free(prime_list);
            prime_list   = NULL;
            prime_count  = 0;
            sieve_limit  = 0;
        }
        build_primes_upto(limit, &prime_list, &prime_count);
        sieve_limit = limit;
    }

    /* scan the primes backwards to find the largest index i
       such that 2*n - prime(i) is also prime.  The array is
       0‑based, while the OEIS index is 1‑based. */
    if (prime_count == 0)
        return (seq_t)0;          /* safety, should not happen for n>=2 */

    long long i = (long long)prime_count - 1;
    for (; i >= 0; --i) {
        uint64_t p = prime_list[(size_t)i];
        if (p > (uint64_t)(limit - 2))
            continue;           /* 2*n - p would be < 2, not prime */
        idx_t diff = limit - p;
        if (is_prime(diff))
            return (seq_t)(i + 1);   /* OEIS uses 1‑based indexing */
    }

    /* by definition such an i exists for every n >= 2 */
    return (seq_t)0;
}
