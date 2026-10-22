/*
 * A309294  (1/2) times the sum of the elements of all subsets of [n] whose
 *          sum is divisible by two.
 * https://oeis.org/A309294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309294.
   Initial values:  a(0)=0, a(1)=0, a(2)=1, a(3)=6, a(4)=20, a(5)=60.
   For n ≥ 6 : a(n) = 6*a(n-1) – 12*a(n-2) + 8*a(n-3).
   The function memoises results so calls may come in any order. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;   /* highest stored index + 1 */
    static idx_t  cache_cap  = 0;   /* current allocated capacity */

    /* initialise cache with first six known terms */
    if (cache == NULL) {
        cache_cap  = 6;
        cache_size = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;      /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)20;
        cache[5] = (seq_t)60;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* ensure enough space for the new terms */
    idx_t needed = n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failed */
        cache = tmp;
        cache_cap = needed;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 6) continue;                /* first six terms already set */
        cache[i] = (6 * cache[i-1]) + (8 * cache[i-3]) - (12 * cache[i-2]);
    }

    cache_size = needed;
    return cache[n];
}
