/*
 * A366387  Divide n by its smallest prime factor, then multiply with the
 *          index of that same prime; a(1) = 0 by convention.
 * https://oeis.org/A366387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n / smallest_prime_factor(n)) *
 *            index_of_the_smallest_prime_factor(n) with a(1) = 0 by
 *            convention. The index of a prime is its 1‑based order among all
 *            primes
 * Range:     u8 n <= 385, u16 n <= 9441, u32 n <= 9441, u64 n <= 9441,
 *            u128 n <= 9441, big any n
 * Helpers:   grow_idx_t_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a lazy sieve to maintain the smallest prime factor (spf) for all
   numbers up to the largest requested index.  It keeps a list of primes and a mapping from
   a prime to its index, enabling O(1) computation of the required term. */

/* Standard headers */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_idx_t_array.h"

/* Static data structures used by the function */
static idx_t current_limit = 0;     /* highest n for which spf is known   */
static idx_t *spf          = NULL; /* spf[i] = smallest prime factor of i   */
static idx_t *prime_index  = NULL; /* prime_index[p] = position of prime p   */
static idx_t *prime_list   = NULL; /* list of primes found so far           */
static size_t prime_count  = 0;     /* number of primes found                 */
static size_t prime_cap    = 0;     /* allocated capacity of prime_list       */

/* Main function required by the assignment. */
seq_t a(idx_t n)
{
    /* n >= 1 for all valid calls.  a(1) = 0 by convention. */
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Expand the sieve if necessary.  The expansion is done once per new limit
       and is linear in the new upper bound.  The algorithm is the classic
       sieve: for each prime p, set spf[m] = p for the first time we reach a
       composite m.  */
    if (n > current_limit) {
        /* Allocate or enlarge auxiliary arrays.  idx_t is a built‑in unsigned integer,
           so it can be safely allocated with malloc/realloc. */
        idx_t new_limit = n;
        spf          = (idx_t *)realloc(spf, (new_limit + 1) * sizeof(idx_t));
        prime_index  = (idx_t *)realloc(prime_index, (new_limit + 1) * sizeof(idx_t));
        if (!spf || !prime_index) abort(); /* allocation error */

        /* Initialise the new entries to default values.  For each i > current_limit
           we assume it is prime until proven otherwise. */
        for (idx_t i = current_limit + 1; i <= new_limit; ++i) {
            spf[i]          = i;   /* default: number itself */
            prime_index[i]  = 0;   /* not a prime yet */
        }

        /* Sieve the range [2, new_limit].  All numbers <= current_limit have
           already been processed; we only need to mark composites in the
           new interval.  We still iterate from 2 to preserve simplicity. */
        for (idx_t i = 2; i <= new_limit; ++i) {
            /* If spf[i] == i then i is prime in the current range. */
            if (spf[i] == i) {
                /* Record the prime if it is a new one. */
                if (i > current_limit) {
                    /* Ensure we have room in the prime list. */
                    if (prime_count == prime_cap) {
                        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                        grow_idx_t_array(&prime_list, &prime_cap, new_cap);
                    }
                    prime_list[prime_count] = i;
                    prime_index[i] = ++prime_count;  /* 1‑based index of the prime */
                }
                /* For i <= sqrt(new_limit), mark its multiples that have not
                   yet been assigned a smaller prime factor. */
                if (i <= new_limit / i) {
                    for (idx_t m = i * i; m <= new_limit; m += i) {
                        if (spf[m] == m) {
                            spf[m] = i;
                        }
                    }
                }
            }
        }

        current_limit = new_limit;
    }

    /* Compute the required term. */
    idx_t p      = spf[n];                     /* smallest prime factor of n */
    idx_t k      = prime_index[p];             /* index of that prime (1‑based) */
    seq_t denom  = (seq_t)p;                   /* convert to seq_t for arithmetic */
    seq_t num    = (seq_t)n / denom;           /* n / p  */
    seq_t result = num * (seq_t)k;             /* final value */
    return result;
}
