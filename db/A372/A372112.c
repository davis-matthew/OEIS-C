/*
 * A372112  Rad-transform of the squarefree numbers A005117 (see Comments).
 * https://oeis.org/A372112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the n-th squarefree number is composite or 1,
 *            otherwise a(n) equals that prime number. The n-th squarefree
 *            number is generated incrementally and cached
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state */
    static bool initialized = false;
    static idx_t max_computed = 0;          /* Largest n for which terms[n] is valid */
    static idx_t current_candidate = 1;     /* Last integer examined for squarefreeness   */
    static seq_t *terms = NULL;             /* Cache of resulting sequence values         */
    static uint64_t primes[2000];           /* List of prime numbers encountered          */
    static size_t prime_cnt = 0;            /* Number of primes stored                    */
    static const idx_t CACHE_SIZE = 20000;  /* Upper limit for precomputation             */

    /* One-time initialization */
    if (!initialized) {
        terms = (seq_t *)calloc(CACHE_SIZE + 1, sizeof(seq_t));
        /* term 1 corresponds to squarefree number 1 */
        terms[1] = (seq_t)1;
        max_computed = 1;
        initialized = true;
    }

    /* Grow the cache if ns beyond current maximum */
    while (max_computed < n) {
        /* Find next squarefree number > current_candidate */
        do {
            current_candidate++;
            /* Test for squarefreeness and primality on the fly */
            bool squarefree = true;
            bool is_prime_candidate = true;
            /* Factorisation using known primes */
            for (size_t j = 0; j < prime_cnt &&
                         (uint64_t)primes[j] * primes[j] <= current_candidate; ++j) {
                uint64_t p = primes[j];
                if (current_candidate % p == 0) {
                    is_prime_candidate = false;           /* composite */
                    if (current_candidate % (p * p) == 0) {
                        squarefree = false;                /* p^2 divides */
                        break;
                    }
                    /* otherwise true squarefree factor p appears once */
                }
            }
            if (!squarefree) {
                continue;                                  /* try next integer */
            }
            /* If squarefree, compute value a(k) */
            if (is_prime_candidate) {
                /* new prime discovered; store it for future tests */
                if (prime_cnt < sizeof(primes) / sizeof(primes[0])) {
                    primes[prime_cnt++] = current_candidate;
                }
            }
            /* Append to sequence cache */
            idx_t idx = ++max_computed;  /* New index */
            if (idx <= CACHE_SIZE) {
                terms[idx] = is_prime_candidate ? (seq_t)current_candidate : (seq_t)1;
            } else {
                /* In case we exceeded the preallocated cache, reallocate */
                size_t new_size = CACHE_SIZE * 2;
                while (idx > new_size) new_size *= 2;
                terms = (seq_t *)realloc(terms, (new_size + 1) * sizeof(seq_t));
                if (!terms) exit(1);                   /* allocation error */
                terms[idx] = is_prime_candidate ? (seq_t)current_candidate : (seq_t)1;
                /* Update CACHE_SIZE if we want to keep track (optional) */
            }
            /* Break out of the DO loop; we have processed this candidate */
            break;
        } while (true);
    }

    return terms[n];
}
