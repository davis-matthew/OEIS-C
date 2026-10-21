/*
 * A275830  a(n) = (2*sqrt(7)*sin(Pi/7))^n + (-2*sqrt(7)*sin(2*Pi/7))^n +
 *          (-2*sqrt(7)*sin(4*Pi/7))^n.
 * https://oeis.org/A275830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -7*a(n-1) + 49*a(n-3) with a(0)=3, a(1)=-7, a(2)=49
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 13, i64 n <= 26, i128 n <= 53,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for size_t, NULL */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) */
#include "../utils/free_seq.h"          /* free_seq(seq_t*) */

seq_t a(idx_t n) {
    /* static cache of previously computed terms */
    static seq_t *cache  = NULL;   /* pointer to sequence array */
    static size_t cache_len = 0;   /* number of computed elements */

    /* initialise cache with first three terms if necessary */
    if (cache_len == 0) {
        cache_len = 3;                         /* need at least indices 0..2 */
        cache = alloc_seq_array(cache_len);
        if (!cache) return 0;                  /* allocation failed, return 0 */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-7;
        cache[2] = (seq_t)49;
    }

    /* if the requested term is already cached, return it */
    if ((size_t)n < cache_len)
        return cache[n];

    /* otherwise, grow the cache to contain the requested term */
    size_t old_len = cache_len;
    size_t new_len = (size_t)n + 1;            /* need indices 0..n */
    seq_t *new_cache = alloc_seq_array(new_len);
    if (!new_cache) return 0;                  /* allocation failed */

    /* copy old values */
    for (size_t i = 0; i < old_len; ++i)
        new_cache[i] = cache[i];

    /* free old cache */
    free_seq(cache);
    cache = new_cache;
    cache_len = new_len;

    /* compute remaining terms using the linear recurrence */
    for (size_t i = old_len; i <= (size_t)n; ++i) {
        /* recurrence only valid for i >= 3, but we start at old_len >= 3 */
        seq_t term = (seq_t)-7 * cache[i - 1] + (seq_t)49 * cache[i - 3];
        cache[i] = term;
    }

    return cache[n];
}
