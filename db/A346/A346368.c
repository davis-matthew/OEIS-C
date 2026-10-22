/*
 * A346368  Odd numbers that can be written in a single way as 2*k^2+p, k>0, p
 *          prime.
 * https://oeis.org/A346368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for odd numbers with a unique representation as 2*k^2 + prime */
/*    OEIS A346368: a(n) = the n-th smallest odd m such that
 *    m = 2*k^2 + p for exactly one pair (k>0, p prime).  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of found terms */
    static size_t cap    = 0;       /* capacity of cache */
    static size_t len    = 0;       /* number of stored terms */
    static seq_t last_od = 1;       /* last odd number tested */
    static idx_t  found   = 0;      /* how many terms have been found */

    /* initialise cache once */
    if (cache == NULL) {
        cap  = 4;            /* start small, grow as needed */
        len  = 0;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* generate terms until we have at least n */
    while (found < n) {
        last_od += (seq_t)2;          /* next odd number */
        seq_t m = last_od;
        idx_t reps = 0;

        /* maximum k satisfies 2*k^2 <= m  ->  k <= sqrt(m/2) */
        seq_t half = m / (seq_t)2;
        idx_t max_k = (idx_t)floor_sqrt(half);

        for (idx_t k = 1; k <= max_k; ++k) {
            seq_t two_k2 = (seq_t)2 * (seq_t)k * (seq_t)k;
            seq_t p = m - two_k2;
            if (p <= 1) continue;
            if (is_prime_big(p)) {
                ++reps;
                if (reps > 1) break;   /* more than one representation */
            }
        }

        if (reps == 1) {                  /* unique representation */
            if (len == cap) {              /* grow cache if needed */
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = m;
            ++found;
        }
    }

    return cache[n - 1];
}
