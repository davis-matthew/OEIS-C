/*
 * A307612  Partial sums of the permutation A307485: one odd, two even, four
 *          odd, eight even, etc.
 * https://oeis.org/A307612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   perm_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of the permutation A307485: one odd, two even, four odd, eight even, etc. */
#include <stdlib.h>   /* malloc, realloc, free */
#include "../utils/perm_at.h"

static seq_t *cache = NULL;
static idx_t  cache_cap = 0;   /* allocated length */
static idx_t  cache_size = 0;  /* largest n computed, cache[0] = 0 */

/* Return a(n): partial sum of the first n elements of the permutation. */
seq_t a(idx_t n)
{
    /* initial allocation */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)cache_cap);
        if (cache == NULL) exit(1);          /* out of memory */
        cache[0] = 0;
        cache_size = 0;
    }

    /* expand cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)cache_cap ? (size_t)cache_cap * 2 : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) exit(1);
        cache = new_cache;
        cache_cap = (idx_t)new_cap;
    }

    /* compute values up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t perm = perm_at(i - 1);         /* term at position i-1 */
        cache[i] = cache[i - 1] + perm;
    }
    cache_size = n;

    return cache[n];
}
