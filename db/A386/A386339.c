/*
 * A386339  Primes without {2, 9} as digits.
 * https://oeis.org/A386339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1603, u32 n <= 1604, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_digit_2_or_9, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A386339: primes whose decimal representation
 * contains no digit 2 or 9.  The function iteratively tests odd
 * candidates and keeps a cache of discovered terms, so repeated calls
 * are fast. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/has_digit_2_or_9.h"

seq_t a(idx_t n)
{
    /* Local static structures that cache already found terms. */
    static seq_t *cache = NULL;     /* array of primes found */
    static size_t   cap   = 0;      /* current capacity of cache    */
    static size_t   len   = 0;      /* number of primes stored      */
    static uint64_t cand  = 3;      /* next candidate (64‑bit counter) */

    /* Allocate first cache block if needed. */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate primes until we have at least n terms cached. */
    while (len < n) {
        /* Skip candidates containing digits 2 or 9. */
        if (has_digit_2_or_9((seq_t)cand)) {
            cand += 2;
            continue;
        }

        /* Test candidate for primality. */
        if (is_prime_big((seq_t)cand)) {
            /* Grow cache if necessary. */
            if (len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* Out of memory – abort the program. */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
            }
            cache[len++] = (seq_t)cand;
        }
        cand += 2;  /* next odd number */
    }

    return cache[n - 1];
}
