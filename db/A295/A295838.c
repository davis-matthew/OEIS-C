/*
 * A295838  Largest value corresponding to a string of n printable ASCII
 *          characters.
 * https://oeis.org/A295838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 256*a(n-1)+126 ; a(0)=0 (OEIS A295838)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache   = NULL;   /* array of computed values          */
    static size_t  size   = 0;       /* number of elements stored        */
    static size_t  capacity = 0;     /* allocated capacity of the array   */

    /* Ensure cache[0] exists (a(0)=0) */
    if (size == 0) {
        cache = (seq_t *)realloc(NULL, sizeof(seq_t) * 8);   /* initial capacity */
        if (!cache) exit(EXIT_FAILURE);
        capacity = 8;
        cache[0] = (seq_t)0;
        size = 1;
    }

    /* If value already cached, return it immediately */
    if (n < size) {
        return cache[n];
    }

    /* Grow the cache to at least (n+1) elements */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity * 2;
        while (new_cap <= (size_t)n + 1) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute from the last cached index up to n */
    for (size_t i = size; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)256 * cache[i-1] + (seq_t)126;
    }
    size = (size_t)n + 1;   /* update cached size */

    return cache[n];
}
