/*
 * A273492  Numbers n such that the average of different permutations of
 *          digits of n is not an integer.
 * https://oeis.org/A273492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   avg_not_integer
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A273492:  Numbers n such that the average of all permutations of its
 * decimal digits is NOT an integer.
 *
 * The average of the distinct permutations of a number with d digits and
 * digit sum S is:
 *
 *        (S * (1 + 10 + 10^2 + ... + 10^(d-1))) / d
 *
 * Thus the average is an integer iff
 *
 *        (S * P(d)) mod d == 0  where  P(d) = Σ_{k=0}^{d-1} 10^k  (mod d).
 *
 * This implementation enumerates integers and returns the n‑th one
 * satisfying the above condition.  For efficiency it stores previously found
 * terms in a static array and continues searching from the last candidate.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/avg_not_integer.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;          /* number of cached terms */
    static idx_t cache_cap = 0;          /* allocated capacity */
    static idx_t next_candidate = 1;     /* next integer to test */

    if (n == 0) return 0;                /* safety, though n>=1 in usage */

    /* extend cache until it contains at least n terms */
    while (cache_len < n) {
        /* find next integer that satisfies the condition */
        while (!avg_not_integer(next_candidate)) {
            next_candidate++;
        }

        /* grow cache if necessary */
        if (cache_len + 1 > cache_cap) {
            cache_cap = cache_cap ? cache_cap << 1 : 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* store the found term */
        cache[cache_len++] = (seq_t)next_candidate;
        next_candidate++;            /* advance for next search */
    }

    /* return the requested term (1‑based index) */
    return cache[n - 1];
}
