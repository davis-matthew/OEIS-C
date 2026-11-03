/*
 * A296933  Primes p such that Legendre(3,p) = 0 or 1.
 * https://oeis.org/A296933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3271, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9506 (2026-11-03)
 * Helpers:   is_prime, mem
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
#include "../utils/mem.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

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
        cache = mem_new(seq_t, capacity);
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
                seq_t *tmp = mem_resize(seq_t, cache, capacity);
                if (tmp == NULL) {  /* realloc failure */
                    mem_free(cache);
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
