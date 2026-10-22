/*
 * A380522  Primes p such that between p and the previous prime there exist 2
 *          distinct integers which are a square and a cube, respectively.
 * https://oeis.org/A380522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   ensure_capacity, floor_cube_root, floor_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Find primes p such that between p and the previous
   prime there exist a square and a cube, with the square integer and
   the cube integer distinct.  This function generates the required
   primes on demand and caches them so that calls with arbitrary
   indices are fast. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;
    static uint64_t last_prev_prime = 2; /* prime before the first processed one */
    /* preallocate a small buffer the first time */
    if (!cache) {
        cache_cap = 0;
        cache = NULL;
    }
    /* generate terms until we have enough */
    while (cache_len < n) {
        uint64_t p = next_prime(last_prev_prime); /* next prime after last_prev_prime */
        uint64_t prev = last_prev_prime;           /* previous prime */
        seq_t low = (seq_t)(prev + 1);
        seq_t high = (seq_t)(p - 1);
        if (low <= high) {
            /* smallest square >= low */
            seq_t s = floor_sqrt(low - 1) + 1;
            seq_t s2 = s * s;
            bool has_square = s2 <= high;
            /* smallest cube >= low */
            seq_t c = floor_cube_root(low - 1) + 1;
            seq_t c3 = c * c * c;
            bool has_cube = c3 <= high;
            bool distinct = false;
            if (has_square && has_cube) {
                if (s2 != c3) {
                    distinct = true;
                } else {
                    seq_t ns = s + 1;
                    if (ns * ns <= high) distinct = true;
                    else {
                        seq_t nc = c + 1;
                        if (nc * nc * nc <= high) distinct = true;
                    }
                }
            }
            if (distinct) {
                /* cache this term */
                ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
                cache[cache_len++] = (seq_t)p;
            }
        }
        last_prev_prime = p;
    }
    return cache[n - 1];
}
