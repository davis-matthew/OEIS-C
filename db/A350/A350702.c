/*
 * A350702  Primes p such that 14*p + 1 divides 2^p - 1.
 * https://oeis.org/A350702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that 14*p+1 divides 2^p - 1 The
 *            function searches sequentially over primes and memoizes the
 *            found ones, so any call order is supported
 * Range:     u8 none, u16 n <= 6, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x) */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) is the first valid prime.  For n==0 behaviour is irrelevant. */
    if (n == (idx_t)0) return (seq_t)0;

    /* Static storage for memoization */
    static idx_t next_candidate = 2;           /* next number to test (prime candidate) */
    static idx_t *qual_primes = NULL;          /* array of qualifying primes */
    static size_t  qual_len   = 0;              /* number of primes stored so far */
    static size_t  qual_cap   = 0;              /* current capacity of the array */

    /* Allocate initial storage if necessary */
    if (qual_primes == NULL) {
        qual_cap = 16;
        qual_primes = (idx_t *)malloc(qual_cap * sizeof(idx_t));
        if (!qual_primes) return (seq_t)0;     /* out-of-memory → return 0 */
    }

    /* Keep searching until we have at least n qualifying primes */
    while (qual_len < n) {
        /* Test candidate only if it is prime */
        if (next_candidate == (idx_t)2 || /* handle the first prime explicitly  */
            (next_candidate % (idx_t)2 == (idx_t)1 && is_prime(next_candidate))) {

            /* Compute the modulus m = 14*p + 1 */
            seq_t m = (seq_t)14 * (seq_t)next_candidate + (seq_t)1;

            /* Compute 2^p mod m */
            seq_t r = mod_pow((seq_t)2, (uint64_t)next_candidate, m);

            /* If r == 1, we have found a qualifying prime */
            if (r == (seq_t)1) {
                /* Resize array if needed */
                if (qual_len == qual_cap) {
                    size_t new_cap = qual_cap * 2;
                    idx_t *tmp = (idx_t *)realloc(qual_primes, new_cap * sizeof(idx_t));
                    if (!tmp) return (seq_t)0; /* out-of-memory -> return 0 */
                    qual_primes = tmp;
                    qual_cap = new_cap;
                }
                qual_primes[qual_len++] = next_candidate;
            }
        }

        /* Advance to the next candidate */
        if (next_candidate == (idx_t)2)
            next_candidate = (idx_t)3;
        else
            next_candidate += (idx_t)2;        /* only odd numbers after 2 are possible primes */
    }

    /* Return the n-th qualifying prime (1-based index) */
    return (seq_t)qual_primes[n - (idx_t)1];
}
