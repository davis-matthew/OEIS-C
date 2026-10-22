/*
 * A398203  Upper (4/3,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5), with
 *            (a(0),a(1),a(2),a(3),a(4)) = (0,3,8,17,30)
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* allocated size of cache   */
    static size_t last_computed = 4;   /* highest index that has been stored */

    /* initial values for indices 0..4 */
    static const seq_t base[5] = {0, 3, 8, 17, 30};

    /* for very small n just return the constant term */
    if (n < 5)
        return base[(size_t)n];

    /* initialise the cache on first call */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0; /* out‑of‑memory: caller should handle */
        for (size_t i = 0; i < 5; ++i)
            cache[i] = base[i];
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = cache_size * 2;
        if (new_size < (size_t)n + 1) new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0; /* out‑of‑memory */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n using the recurrence */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        cache[i] =
            2 * cache[i-1]
          - cache[i-2]
          + cache[i-3]
          - 2 * cache[i-4]
          + cache[i-5];
    }
    last_computed = (size_t)n;

    return cache[(size_t)n];
}
