/*
 * A279392  Bisection of primes congruent to 1 modulo 4 (A002144), depending
 *          on the corresponding sum of the A002972 and 2*A002973 entries
 *          being congruent to 1 modulo 4 or not. Here we give the first case.
 * https://oeis.org/A279392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime ≡1 (mod 4) for which the representation * p
 *            = A² + B² (with A odd, B even) satisfies (A + B) ≡ 1 (mod 4).
 *            The * implementation builds the sequence lazily by testing
 *            successive * primes of the form 4k+1 and collecting those that
 *            meet the congruence * condition. A small cache h...
 * Range:     u8 n <= 4, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/is_prime_small.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Static cache for the computed terms */
    static seq_t  *cache      = NULL;
    static size_t cache_len   = 0;
    static size_t cache_cap   = 0;

    /* Current candidate to test for primality */
    static idx_t next_candidate = 5;   /* 5 is the first prime ≡1 (mod 4) */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
    }

    /* Extend cache until it contains at least n entries */
    while (cache_len < (size_t)n) {
        /* Find the next prime ≡1 (mod 4) */
        while (true) {
            if (next_candidate < 5) next_candidate = 5;   /* safety */
            if (is_prime_small(next_candidate) && (next_candidate % 4 == 1U)) {
                seq_t p = (seq_t)next_candidate;          /* current prime */

                /* Find its unique representation p = A^2 + B^2 with A odd, B even */
                seq_t A = 0, B = 0;
                seq_t limit = floor_sqrt(p);
                for (seq_t a = 1; a <= limit; a += 2) {    /* a odd */
                    seq_t a2 = a * a;
                    if (a2 > p) break;
                    seq_t b2 = p - a2;
                    seq_t b  = floor_sqrt(b2);
                    if (b * b == b2 && (b & 1) == 0) { /* B even */
                        A = a;
                        B = b;
                        break;
                    }
                }

                /* If the congruence holds, store the prime */
                if (A != 0 && ((A + B) & 3) == 1) {
                    if (cache_len == cache_cap) {
                        cache_cap <<= 1;
                        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                        if (tmp == NULL) exit(EXIT_FAILURE);
                        cache = tmp;
                    }
                    cache[cache_len++] = p;
                }
            }
            /* Advance to next candidate */
            next_candidate++;

            /* Break if we've just added enough terms */
            if (cache_len >= (size_t)n) break;
        }
    }

    /* Return the desired term (1‑based indexing) */
    return cache[n - 1];
}
