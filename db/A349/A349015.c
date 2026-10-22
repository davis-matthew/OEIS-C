/*
 * A349015  G.f. A(x) satisfies: A(x) = 1 + x * A(x) / (1 - x) - x * A(x)^2.
 * https://oeis.org/A349015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} a(k) * (1 - a(n-k-1)) for n > 0
 *            The sequence is memoized in a static table to allow arbitrary
 *            order of calls with O(n^2) construction time
 * Range:     i8 n <= 11, i16 n <= 18, i32 n <= 31, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* static table of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache with a(0) */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_len);
        cache[0] = 1;
    }

    /* extend cache up to n if necessary */
    if ((idx_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k < (idx_t)i; ++k) {
                seq_t term = cache[k] * ((seq_t)1 - cache[i - 1 - k]);
                sum += term;
            }
            cache[i] = sum;
        }
        cache_len = new_len;
    }

    return cache[n];
}
