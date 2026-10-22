/*
 * A354449  a(n) is the number of pairs of primes (p,q) with p<q such that p+q
 *          = 2*n and that 2*n+p, 2*n+q, p*q-2*n and p*q+2*n are primes.
 * https://oeis.org/A354449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1967, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A354449
   Implementation is a straightforward search:
     For each n >= 1, consider every prime p < n such that q = 2n - p is prime.
     Count the pair (p, q) iff all of
          2n+p, 2n+q, p*q-2n, p*q+2n
     are also prime.  Use a simple sieve to generate all primes up to 2n
     (cached for reuse across calls).  Primality tests for large numbers
     (p*q ± 2n) are performed using isPrimeGeneric() from the utilities. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

/* static cache of primes up to the largest 2n seen so far */
static uint64_t *prime_list = NULL;    /* array of primes */
static size_t    prime_cnt  = 0;        /* number of primes in the array */
static size_t    prime_limit = 0;      /* the maximum n such that 2n is covered */

/* build all primes <= 2*limit using a simple sieve (cached) */
static void build_prime_cache(idx_t limit)
{
    size_t needed_limit = (size_t)limit * 2;
    if (needed_limit <= prime_limit) return;   /* already computed */

    /* allocate flag array */
    int *is_prime = (int*)malloc((needed_limit + 1) * sizeof(int));
    if (!is_prime) return;                     /* out of memory – give up */

    for (size_t i = 0; i <= needed_limit; ++i) is_prime[i] = 1;
    if (needed_limit >= 0) is_prime[0] = 0;
    if (needed_limit >= 1) is_prime[1] = 0;

    for (size_t i = 2; i * i <= needed_limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= needed_limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* count primes */
    size_t count = 0;
    for (size_t i = 2; i <= needed_limit; ++i) if (is_prime[i]) ++count;

    /* allocate and fill prime list */
    uint64_t *new_list = (uint64_t*)malloc(count * sizeof(uint64_t));
    if (!new_list) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= needed_limit; ++i) {
        if (is_prime[i]) {
            new_list[idx++] = i;
        }
    }

    free(is_prime);
    free(prime_list);
    prime_list = new_list;
    prime_cnt  = count;
    prime_limit = (size_t)limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – undefined, return 0 */

    build_prime_cache(n);          /* ensure primes up to 2n are available */

    seq_t two_n = (seq_t)n * 2;
    seq_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)prime_list[i];
        if (p >= (seq_t)n) break;     /* only need p < n so that q > p */

        seq_t q = two_n - p;
        if (!isPrimeGeneric(q)) continue;
        if (!isPrimeGeneric(two_n + p)) continue;
        if (!isPrimeGeneric(two_n + q)) continue;

        seq_t prod = p * q;
        if (!isPrimeGeneric(prod - two_n)) continue;
        if (!isPrimeGeneric(prod + two_n)) continue;

        ++count;
    }

    return count;
}
