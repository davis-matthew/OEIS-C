/*
 * A259190  Primes of the form sigma(n) + sigma(n)^2 - 1.
 * https://oeis.org/A259190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 3843, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A259190: a(n) is the n-th prime of the form σ(k) + σ(k)^2 – 1,
   where σ(k) is the sum of divisors of k.  The primes are collected
   in the order of increasing k. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/sigma_of.h"      /* σ(n) ; returns seq_t */
#include "../utils/isPrimeGeneric.h" /* bool isPrimeGeneric(seq_t) */

/* Return the n‑th term of the sequence for n >= 1. */
seq_t a(idx_t n)
{
    /* --------------------------------------------------------------------
     *  Static state for memoisation
     * -------------------------------------------------------------------- */
    static seq_t *primes      = NULL;      /* found primes  */
    static size_t primes_len  = 0;          /* number of stored primes */
    static size_t primes_cap  = 0;          /* allocated capacity     */

    static idx_t current_k = 0;             /* last k processed       */

    /* ------------------------------------------------------------
     *  Grow the prime array to accommodate at least n primes
     * ------------------------------------------------------------ */
    while (primes_len < n) {
        /* --- next k to test --- */
        ++current_k;

        /* --- compute candidate --- */
        seq_t sigma = sigma_of(current_k);
        seq_t candidate = sigma * sigma + sigma - (seq_t)1;

        /* --- test primality --- */
        if (isPrimeGeneric(candidate)) {
            /* grow primes array if necessary */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap << 1 : 64;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* OOM: return 0 (undefined behaviour not required to be handled) */
                    free(primes);
                    primes = NULL;
                    primes_cap = primes_len = 0;
                    return (seq_t)0;
                }
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = candidate;
        }
    }

    return primes[n - 1];
}
