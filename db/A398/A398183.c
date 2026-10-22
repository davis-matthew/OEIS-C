/*
 * A398183  a(n) = second-smallest prime p <= n such that 2n - p is also
 *          prime, or 0 if fewer than two such primes exist.
 * https://oeis.org/A398183
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the second–smallest prime p ≤ n such that 2n – p is
 *            prime, or 0 if fewer than two such primes exist
 * Range:     u8 n <= 335, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t n) */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n) */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit,
                                         uint64_t **primes_out, size_t *cnt_out) */

seq_t a(idx_t n)
{
    static uint64_t *primes       = NULL;  /* array of primes ≤ current limit   */
    static size_t   prime_count   = 0;     /* number of primes in the array      */
    static idx_t    current_limit = 0;     /* last limit for which primes were built */

    /* If we have not yet built primes up to n, do so now. */
    if (n > current_limit) {
        /* Free any old prime list. */
        if (primes != NULL)
            free(primes);

        /* Build primes ≤ n.  The helper allocates a new array and returns it
           through the pointer arguments. */
        build_primes_upto(n, &primes, &prime_count);
        current_limit = n;
    }

    /* Find the second smallest prime p ≤ n for which 2n – p is prime. */
    seq_t second_prime = 0;
    size_t matched     = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];          /* p is ≤ n by construction          */
        seq_t candidate = ((seq_t)2 * (seq_t)n) - (seq_t)p;

        /* Test primality of candidate.  The helper works for arbitrary
           precision seq_t values.  For seq_t that happens to fit in 64 bits,
           it automatically calls the native 64‑bit test. */
        if (is_prime_big(candidate)) {
            ++matched;
            if (matched == 2) {
                second_prime = (seq_t)p;
                break;
            }
        }
    }

    return second_prime;   /* 0 if fewer than two matches were found          */
}
