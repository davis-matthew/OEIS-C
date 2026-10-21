/*
 * A295860  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -2,
 *          a(1) = 1, a(2) = 0, a(3) = 1.
 * https://oeis.org/A295860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 44, i64 n <= 90, i128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4)
   with initial values: a(0) = -2, a(1) = 1, a(2) = 0, a(3) = 1 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Dynamically grow a cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* Number of terms stored so far */
    static size_t cache_cap = 0;   /* Allocated capacity */

    /* Ensure base cases are present */
    if (cache_len == 0) {
        cache_cap = 8; /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed; caller will likely fail too */
        cache[0] = (seq_t)-2;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache_len = 4;
    }

    /* If the requested index is already cached, return it directly */
    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* Grow cache capacity if necessary */
    size_t required = (size_t)(n + 1);
    if (required > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < required) new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from the current cache length up to n */
    for (size_t i = cache_len; i <= required - 1; ++i) {
        /* recurrence indices i-1, i-2, i-3, i-4 are safe because i >= 4 here */
        seq_t val = cache[i - 1]
                    + (seq_t)3 * cache[i - 2]
                    - (seq_t)2 * cache[i - 3]
                    - (seq_t)2 * cache[i - 4];
        cache[i] = val;
    }
    cache_len = required;

    return cache[(size_t)n];
}
