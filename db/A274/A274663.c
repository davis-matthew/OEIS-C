/*
 * A274663  Sum of n-th powers of the roots of x^3 + 4*x^2 - 11*x - 1.
 * https://oeis.org/A274663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4*a(n-1) + 11*a(n-2) + a(n-3), a(0) = 3, a(1) = -4, a(2)
 *            = 38
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 12, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* size of allocated buffer */
    static size_t cache_len = 0;      /* number of elements actually stored */

    /* Allocate or enlarge the cache if necessary */
    if (cache == NULL) {
        /* Ensure room for at least the first three terms */
        size_t sz = (size_t)n + 1;
        if (sz < 3) sz = 3;
        cache = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory: signal by 0 */

        /* Initialise the base cases */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-4;
        cache[2] = (seq_t)38;
        cache_len = 3;
        cache_cap = sz;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out‑of‑memory: signal by 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    while (cache_len <= (size_t)n) {
        cache[cache_len] = ((seq_t)-4) * cache[cache_len - 1]
                         + ((seq_t)11) * cache[cache_len - 2]
                         + cache[cache_len - 3];
        cache_len++;
    }

    return cache[n];
}
