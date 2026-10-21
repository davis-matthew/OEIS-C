/*
 * A248554  Numbers k such that the sum of the digital roots of the first k
 *          primes is also prime; increasingly sorted.
 * https://oeis.org/A248554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) – Numbers k such that the sum of the digital
 * roots of the first k primes is prime.
 * Computation: iterate primes, keep a running sum of their
 * digital roots, record the prime index whenever the sum is
 * prime.  Results are memoised so that any call order runs in
 * O(k) time, where k is the maximal n requested.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime.h"          /* bool is_prime(idx_t x);          */
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n);    */

seq_t a(idx_t n)
{
    /* Memoised state */
    static seq_t *terms = NULL;   /* array of found k values          */
    static size_t cap = 0;        /* allocated capacity of terms      */
    static size_t len = 0;        /* number of terms currently stored */
    static idx_t candidate = 2;   /* next integer to test for prime   */
    static idx_t primes_seen = 0; /* how many primes have been processed */
    static seq_t sum_dr = 0;      /* cumulative sum of digital roots  */

    /* Initialise storage on first call */
    if (!terms) {
        cap = 16;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) return 0;     /* allocation failure – return zero */
    }

    /* If the requested index has already been computed, return it */
    if (n >= 1 && (size_t)(n - 1) < len)
        return terms[(size_t)(n - 1)];

    /* Generate primes until we have at least n terms in the sequence */
    while (len < n) {
        /* Check whether the current candidate is prime */
        bool is_prime_candidate = false;
        if (candidate == 2) {
            is_prime_candidate = true;
        } else if ((candidate & 1) && is_prime(candidate)) {
            is_prime_candidate = true;
        }

        if (is_prime_candidate) {
            primes_seen++;                 /* count this prime */

            /* Digital root of the prime */
            seq_t dr = (candidate % 9 == 0) ? 9 : (seq_t)(candidate % 9);

            /* Update running sum of digital roots */
            sum_dr += dr;

            /* If the sum is prime, record the current prime index */
            if (isPrimeGeneric(sum_dr)) {
                /* Grow the terms array if needed */
                if (len >= cap) {
                    size_t new_cap = cap << 1;
                    if (new_cap == 0) new_cap = 16;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;           /* reallocation failed */
                    terms = tmp;
                    cap = new_cap;
                }
                terms[len++] = primes_seen;
            }
        }

        /* Advance to the next candidate */
        if (candidate == 2) {
            candidate = 3;           /* after 2, skip even numbers */
        } else {
            candidate += 2;
        }
    }

    return terms[(size_t)(n - 1)];
}
