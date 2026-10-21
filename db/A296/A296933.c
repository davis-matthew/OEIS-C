/*
 * A296933  Primes p such that Legendre(3,p) = 0 or 1.
 * https://oeis.org/A296933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3271, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime p (n≥1) such that the Legendre
 * symbol (3|p) is 0 or 1.  Equivalently, p=3 or p is a prime with 3 a
 * quadratic residue modulo p.  The sequence is the OEIS sequence
 * A296933.  It is computed by sieving successive odd integers,
 * testing primality with the provided is_prime() routine, and
 * testing the Legendre symbol with jacobi_symbol(3,p).  The result is
 * cached so that repeated calls in any order require no recomputation.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* Lazy, static cache of primes satisfying the condition. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t count   = 0;          /* number of terms stored in cache   */
    static uint64_t next_candidate = 3; /* next odd number to test           */

    /* Initialise cache lazily. */
    if (cache == NULL) {
        capacity = 64;                /* initial capacity */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL)           /* allocation failure */
            return (seq_t)0;
    }

    /* Generate terms until the cache contains at least n items. */
    while (count < n) {
        /* Test current candidate for being a prime in the desired class. */
        if (is_prime(next_candidate) &&
            (next_candidate == 3 || jacobi_symbol(3, next_candidate) == 1)) {
            /* Expand cache if necessary. */
            if (count == capacity) {
                capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
                if (tmp == NULL) {  /* realloc failure */
                    free(cache);
                    cache = NULL;
                    return (seq_t)0;
                }
                cache = tmp;
            }
            cache[count++] = (seq_t)next_candidate;
        }

        /* Advance to the next odd number (skip even numbers). */
        if (next_candidate == 2)          /* should never happen */
            next_candidate = 3;
        else if (next_candidate == 3)
            next_candidate = 5;
        else
            next_candidate += 2;
    }

    return cache[n - 1];
}
