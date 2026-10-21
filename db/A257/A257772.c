/*
 * A257772  Numbers n>=0 such that (n+1)^3 - n^3 = 3*n^2+3*n+1 is not prime.
 * https://oeis.org/A257772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 2867, u32 n <= 2867, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A257772: numbers n≥0 such that 3*n^2 + 3*n + 1 is not prime */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* static cache of values */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_count   = 0;
    static idx_t next_candidate = 0;  /* next n to test */

    /* grow the cache as needed */
    if (cache == NULL) {
        cache_capacity = 64;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    }

    /* generate terms until we have at least n terms */
    while (cache_count < (size_t)n) {
        seq_t cand = (seq_t)next_candidate;
        seq_t val = 3 * cand * cand + 3 * cand + 1;
        if (!is_prime_big(val)) {           /* composite or 1 */
            if (cache_count == cache_capacity) {   /* expand */
                cache_capacity <<= 1;
                cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
            }
            cache[cache_count++] = cand;
        }
        ++next_candidate;
    }

    return cache[(size_t)(n - 1)];
}
