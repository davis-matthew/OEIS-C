/*
 * A254760  Fundamental positive solution x = x1(n) of the first class of the
 *          Pell equation x^2 - 2*y^2 = A007519(n), n>=1 (primes congruent to
 *          1 mod 8).
 * https://oeis.org/A254760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the value a(n) of A254760:
 *
 *  a(n) is the smallest positive integer x such that
 *  there exists an integer y with
 *
 *          x^2 - 2*y^2 = p
 *
 *  where p is the n-th prime congruent to 1 (mod 8).
 *
 *  The function generates primes on demand, caches the computed
 *  a(n) values, and for each qualifying prime p it finds the
 *  minimal solution by searching the smallest y for which
 *  p + 2*y^2 is a perfect square.  This is efficient for n ≤ 10⁴.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

/* Helper: minimal x for a given prime p (p ≡ 1 (mod 8)) */
static seq_t minimal_solution(seq_t p)
{
    for (seq_t y = 1; ; ++y) {
        seq_t t = p + 2 * y * y;           /* t = p + 2*y^2  */
        seq_t x = floor_sqrt(t);           /* integer sqrt of t  */
        if (x * x == t)                    /* perfect square?   */
            return x;                      /* minimal x found   */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Persistent cache of computed values and primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* capacity of cache array   */
    static size_t cache_len = 0;     /* number of values stored   */

    /* Small primes list used for primality tests */
    static idx_t *small_primes = NULL;
    static size_t sp_cap = 0;
    static size_t sp_len = 0;

    /* Last odd integer tested for primality */
    static idx_t candidate = 3;          /* start with the first odd number */

    /* Expand the cache to accommodate n entries if needed */
    if (cache_len < n) {
        size_t needed = n;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < needed) new_cap = needed;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Expand the small primes list if required */
        if (sp_cap < cache_len + 10) {   /* some spare capacity */
            size_t new_sp_cap = sp_cap ? sp_cap * 2 : 16;
            small_primes = (idx_t *)realloc(small_primes,
                                            new_sp_cap * sizeof(idx_t));
            sp_cap = new_sp_cap;
        }

        /* Generate primes until we have n values */
        while (cache_len < n) {
            /* Test whether candidate is prime */
            bool is_prime = true;
            for (size_t i = 0; i < sp_len; ++i) {
                idx_t p = small_primes[i];
                if (p * p > candidate) break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime) {
                /* Record the prime for future divisibility tests */
                if (sp_len == sp_cap) {
                    size_t new_sp_cap = sp_cap ? sp_cap * 2 : 16;
                    small_primes = (idx_t *)realloc(small_primes,
                                                    new_sp_cap * sizeof(idx_t));
                    sp_cap = new_sp_cap;
                }
                small_primes[sp_len++] = candidate;

                /* If this prime is ≡1 (mod 8), compute its a(n) value */
                if ((candidate & 7) == 1) {
                    seq_t x_val = minimal_solution((seq_t)candidate);
                    cache[cache_len++] = x_val;
                }
            }
            candidate += 2;   /* only odd numbers need to be tested */
        }
    }
    return cache[n - 1];
}
