/*
 * A399357  Number of independent vertex sets in the 4 X n Klein bottle graph.
 * https://oeis.org/A399357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n‑1) + 12*a(n‑2) - 4*a(n‑3) - 3*a(n‑4) + a(n‑5);
 *            a(0)=1, a(1)=7, a(2)=21, a(3)=151, a(4)=673, a(5)=3727
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      // dynamic cache for computed terms
static size_t cache_cap = 0;     // capacity of cache array
static size_t last_computed = 0; // highest index that has been filled

seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n < 6) {
        static const seq_t base[6] = {1, 7, 21, 151, 673, 3727};
        return base[n];
    }

    /* Ensure the cache is large enough */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = needed;
        if (new_cap < 16) new_cap = 16;          /* start with a modest size */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                      /* malloc failure: return 0 */
        cache = tmp;
        cache_cap = new_cap;

        /* If this is the first time, initialise the first six values */
        if (last_computed == 0) {
            cache[0] = 1;   cache[1] = 7;   cache[2] = 21;
            cache[3] = 151; cache[4] = 673; cache[5] = 3727;
            last_computed = 5;
        }
    }

    /* Compute any missing terms up to n */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        /* Compute in two steps to avoid negative intermediate values */
        seq_t val = 3 * cache[i - 1] + 12 * cache[i - 2] + cache[i - 5];
        seq_t sub = 4 * cache[i - 3] + 3 * cache[i - 4];
        cache[i] = val - sub;
    }
    last_computed = (size_t)n;
    return cache[n];
}
