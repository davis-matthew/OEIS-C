/*
 * A340767  Number of primes p <= 6*n + 5 that are congruent to 2 modulo 3.
 * https://oeis.org/A340767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 601, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS sequence A340767
 * Formula: a(n) = number of primes p <= 6*n+5 that are congruent to 2 (mod 3)
 * This implementation uses a simple sieve and cumulative counts, recomputed only
 * when a larger limit is requested.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* external linkage as required */
seq_t a(idx_t n)
{
    /* compute the upper bound 6*n + 5 */
    idx_t limit = n * 6ULL + 5ULL;
    /* static state: sieve, cumulative counts, current maximum limit */
    static uint8_t *is_prime = NULL;
    static seq_t *cum       = NULL;
    static size_t  sieve_size = 0;   /* current allocated size for is_prime */
    static idx_t   current_limit = 0;/* maximum limit for which sieve was built */

    if (limit > current_limit) {
        /* allocate or reallocate arrays to accommodate the new limit */
        size_t new_size = (size_t)limit + 1;  /* indices 0..limit */

        /* free old arrays if any (not strictly necessary) */
        if (is_prime) free(is_prime);
        if (cum)      free(cum);

        is_prime = (uint8_t*)malloc(new_size);
        cum     = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!is_prime || !cum) {
            /* allocation failure: return 0 as a safe fallback */
            return 0;
        }

        /* initialize: mark all entries as prime initially */
        for (size_t i = 0; i < new_size; ++i)
            is_prime[i] = 1;
        if (new_size > 0) is_prime[0] = 0;
        if (new_size > 1) is_prime[1] = 0;

        /* simple Eratosthenes sieve */
        for (size_t p = 2; p * p <= limit; ++p) {
            if (is_prime[p]) {
                for (size_t mult = p * p; mult <= limit; mult += p)
                    is_prime[mult] = 0;
            }
        }

        /* build cumulative counts of primes ≡ 2 (mod 3) */
        cum[0] = 0;
        for (size_t i = 1; i <= limit; ++i) {
            cum[i] = cum[i-1];
            if (is_prime[i] && (i % 3 == 2))
                cum[i] = cum[i] + 1;
        }

        /* update state */
        sieve_size    = new_size;
        current_limit = limit;
    }

    /* return the cumulative count up to the requested limit */
    return cum[limit];
}
