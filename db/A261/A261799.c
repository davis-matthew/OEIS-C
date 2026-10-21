/*
 * A261799  Number of 7-compositions of n: matrices with 7 rows of nonnegative
 *          integers with positive column sums and total element sum n.
 * https://oeis.org/A261799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is given by the linear recurrence derived from the
 *            generating function G(x) = (1-x)^7 / (2(1-x)^7 - 1): a(0..7) =
 *            1, 7, 77, 819, 8687, 92141, 977347, 10366833 for n ≥ 8: a(n) =
 *            14 a(n-1) - 42 a(n-2) + 70 a(n-3) - 70 a(n-4) + 42 a(n-5) - 14
 *            a(n-6) + 2 a(n-7)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic cache of computed terms */
static idx_t  cache_len = 0;         /* allocated size of cache */
static idx_t  max_cached = 7;        /* highest index that has a valid value */

static const seq_t BASE[8] = {
    (seq_t)1,
    (seq_t)7,
    (seq_t)77,
    (seq_t)819,
    (seq_t)8687,
    (seq_t)92141,
    (seq_t)977347,
    (seq_t)10366833
};

seq_t a(idx_t n)
{
    /* initialize cache with first 8 terms */
    if (cache_len == 0) {
        cache_len = 8;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* out of memory: return 0 */
        for (idx_t i = 0; i < 8; ++i) cache[i] = BASE[i];
    }

    /* if value already computed, just return it */
    if (n <= max_cached) return cache[n];

    /* extend cache size if necessary (doubling strategy) */
    if (n + 1 > cache_len) {
        idx_t new_len = (n + 1 > cache_len * 2) ? (n + 1) : (cache_len * 2);
        seq_t *temp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!temp) return (seq_t)0;          /* realloc failed */
        cache = temp;
        cache_len = new_len;
    }

    /* compute new terms using recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] =
            14 * cache[i-1]
          - 42 * cache[i-2]
          + 70 * cache[i-3]
          - 70 * cache[i-4]
          + 42 * cache[i-5]
          - 14 * cache[i-6]
          +  2 * cache[i-7];
    }
    max_cached = n;
    return cache[n];
}
