/*
 * A306968  Numbers prime to 7 that are not represented by x^2+y^2+7*z^2.
 * https://oeis.org/A306968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the sequence A306968: numbers that are prime to 7 and
   not representable as x^2 + y^2 + 7*z^2 with integers x,y,z.
   The implementation uses a simple brute‑force check for representability
   and memoises the sequence values so that repeated calls are fast.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* static cache state */
static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_len = 0;         /* number of cached values */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t next_candidate = 1;     /* next integer to test */

/* append a value to the cache, expanding storage if necessary */
static void push_cache(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort(); /* memory exhausted */
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* test whether `m` can be written as x^2 + y^2 + 7*z^2 */
static bool is_representable(seq_t m) {
    /* iterate over all possible z */
    for (idx_t z = 0; ; ++z) {
        seq_t term = (seq_t)7 * (seq_t)z * (seq_t)z;
        if (term > m) break;
        seq_t rem1 = m - term;
        /* iterate over all possible x */
        for (idx_t x = 0; ; ++x) {
            seq_t term2 = (seq_t)x * (seq_t)x;
            if (term2 > rem1) break;
            seq_t rem2 = rem1 - term2;
            seq_t s = floor_sqrt(rem2);
            if (s * s == rem2) return true;   /* found a solution */
        }
    }
    return false;   /* no solution found */
}

/* external entry point, giving the nth term (n >= 1) */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, return 0 */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        idx_t m = next_candidate;
        for (;;) {
            if (m % 7 != 0) {   /* test only numbers coprime to 7 */
                if (!is_representable((seq_t)m)) {
                    push_cache((seq_t)m);
                    next_candidate = m + 1;
                    break;
                }
            }
            ++m;
        }
    }
    return cache[n - 1];
}
