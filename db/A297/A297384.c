/*
 * A297384  Number of Eulerian cycles in the n-antiprism graph.
 * https://oeis.org/A297384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 48*a(n-2) + 52*a(n-3) - 16*a(n-4) a(1) = 4,
 *            a(2) = 44, a(3) = 372, a(4) = 2932
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] holds a(1), cache[1] holds a(2), … */
    static seq_t *cache = NULL;     /* allocated cache for computed terms */
    static idx_t max_n = 0;         /* current maximum n for which cache holds a(n) */

    if (n == 0) return 0;           /* n < 1 is undefined, guard */

    /* First call – initialise cache with the base values */
    if (cache == NULL) {
        max_n = 4;                  /* we know the first four terms */
        cache = (seq_t *)calloc(max_n, sizeof(seq_t));
        cache[0] = (seq_t)4;        /* a(1) */
        cache[1] = (seq_t)44;       /* a(2) */
        cache[2] = (seq_t)372;      /* a(3) */
        cache[3] = (seq_t)2932;     /* a(4) */
    }

    /* If a higher n is requested, grow the cache and compute the new terms */
    if (n > max_n) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (new_cache == NULL) {           /* allocation failure */
            /* fall back – return 0 for safety */
            return 0;
        }
        cache = new_cache;
        /* compute terms from max_n+1 up to n using the recurrence */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            cache[i - 1] = (seq_t)13 * cache[i - 2]
                          - (seq_t)48 * cache[i - 3]
                          + (seq_t)52 * cache[i - 4]
                          - (seq_t)16 * cache[i - 5];
        }
        max_n = n;
    }

    /* Return the requested term */
    return cache[n - 1];
}
