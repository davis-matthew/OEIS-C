/*
 * A365581  Number of dominating sets in the n-double cone graph.
 * https://oeis.org/A365581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            6*a(n-1)-5*a(n-2)-6*a(n-3)-25*a(n-4)+4*a(n-5)-a(n-6)+4*a(n-7);
 *            Initial values: a(1) = 5, a(2) = 25, a(3) = 113, a(4) = 377,
 *            a(5) = 1465, a(6) = 5617, a(7) = 21425
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of a(i), index 0 unused */
static idx_t cache_cap = 0;   /* largest index for which cache is valid */

/* Ensure cache is computed up to at least n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    /* Allocate more space if needed */
    if (cache == NULL) {
        /* Allocate space for indices 0..7 first */
        const idx_t init_size = 8;   /* 0..7 inclusive */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        /* Initial known values */
        cache[0] = 0;
        cache[1] = 5;
        cache[2] = 25;
        cache[3] = 113;
        cache[4] = 377;
        cache[5] = 1465;
        cache[6] = 5617;
        cache[7] = 21425;
        cache_cap = 7;
    }

    /* Grow the buffer to hold indices up to n */
    if (n > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) /* allocation failure */
            exit(EXIT_FAILURE);
        cache = new_cache;

        /* Compute new terms using the recurrence */
        for (idx_t i = cache_cap + 1; i <= n; ++i) {
            seq_t val =
                6 * cache[i - 1]
              - 5 * cache[i - 2]
              - 6 * cache[i - 3]
              - 25 * cache[i - 4]
              + 4 * cache[i - 5]
              - cache[i - 6]
              + 4 * cache[i - 7];
            cache[i] = val;
        }
        cache_cap = n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, undefined for n=0; return 0 */
        return 0;
    if (n <= 7) {
        /* Return the pre‑computed base values directly */
        static const seq_t init[7] = {
            5, 25, 113, 377, 1465, 5617, 21425
        };
        return init[n - 1];
    }
    ensure_cache(n);
    return cache[n];
}
