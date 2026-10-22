/*
 * A316178  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:
 * a(1..12) = {4, 30, 134, 601, 2665, 11915, 53134, 237241, 1058821, 4726432, 
 *             21096932, 94170565}
 * for n > 12:
 * a(n) = 3*a(n-1) + 8*a(n-2) - 4*a(n-3) - 11*a(n-4) 
 *        - a(n-5) - 13*a(n-7) + 2*a(n-8) - a(n-9) - a(n-10) - a(n-11)
 */

#include <stdlib.h>

/* static cache for memoised values */
static seq_t *cache      = NULL;
static size_t cache_size = 0;

/* initial values a(1)..a(12) */
static const seq_t base_vals[12] = {
    (seq_t)4,   (seq_t)30,  (seq_t)134, (seq_t)601,
    (seq_t)2665, (seq_t)11915, (seq_t)53134, (seq_t)237241,
    (seq_t)1058821, (seq_t)4726432, (seq_t)21096932, (seq_t)94170565
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* behaviour below offset is irrelevant */

    /* initialise cache with the first 12 terms if needed */
    if (cache_size < 12) {
        cache = (seq_t*)malloc(12 * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */
        for (size_t i = 0; i < 12; ++i) cache[i] = base_vals[i];
        cache_size = 12;
    }

    /* if already computed, return */
    if ((size_t)n <= cache_size) return cache[n - 1];

    /* grow cache up to n */
    size_t new_size = (size_t)n;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0; /* allocation failed */
    cache = tmp;

    /* compute missing terms */
    for (size_t j = cache_size; j < new_size; ++j) {
        seq_t val = (seq_t)3 * cache[j - 1]
                  + (seq_t)8 * cache[j - 2]
                  - (seq_t)4 * cache[j - 3]
                  - (seq_t)11 * cache[j - 4]
                  -           cache[j - 5]
                  - (seq_t)13 * cache[j - 7]
                  + (seq_t)2 * cache[j - 8]
                  -           cache[j - 9]
                  -           cache[j - 10]
                  -           cache[j - 11];
        cache[j] = val;
    }

    cache_size = new_size;
    return cache[n - 1];
}
