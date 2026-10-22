/*
 * A395607  a(n) = a(n-1) + a(n-3) + a(n-4), a(0) = 1, a(1) = 6, a(2) = 36,
 *          a(3) = 42.
 * https://oeis.org/A395607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-3) + a(n-4),
   a(0) = 1, a(1) = 6, a(2) = 36, a(3) = 42 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;          /* pointer to array of seq_t */
    static size_t capacity = 0;          /* allocated size of array   */
    static size_t length   = 0;          /* number of valid entries   */

    /* Ensure the cache is initialized with the first 4 terms */
    if (cache == NULL) {
        capacity = 4;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)42;
        length   = 4;
    }

    /* If n already computed, return it immediately */
    if ((idx_t)length > n) {
        return cache[(size_t)n];
    }

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity;
        if (new_cap == 0) new_cap = 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure: return 0 as fallback */
            return (seq_t)0;
        }
        cache     = tmp;
        capacity  = new_cap;
    }

    /* Compute missing values up to n using the recurrence */
    for (size_t i = length; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
    }

    length = (size_t)n + 1;   /* update the number of valid entries */
    return cache[(size_t)n];
}
