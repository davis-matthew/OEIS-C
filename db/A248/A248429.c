/*
 * A248429  Number of length n+2 0..4 arrays with every three consecutive
 *          terms having the sum of some two elements equal to twice the
 *          third.
 * https://oeis.org/A248429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-3) - 4*a(n-4), a(1)=29, a(2)=45, a(3)=69,
 *            a(4)=101
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 41, u64 n <= 89, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence holds for n >= 5. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n <= 4) {
        switch (n) {
            case 1: return 29;
            case 2: return 45;
            case 3: return 69;
            case 4: return 101;
        }
    }

    /* Memoised computation for n >= 5. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of values stored in cache (>=4 after init) */

    /* Initialise cache to the first four known values if necessary. */
    if (cache == NULL || cache_size < 4) {
        if (cache != NULL)
            free(cache);
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (cache == NULL)
            abort();          /* allocation failed */
        cache[0] = 29;
        cache[1] = 45;
        cache[2] = 69;
        cache[3] = 101;
        cache_size = 4;
    }

    /* Extend the cache up to the requested index, if needed. */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (new_cache == NULL)
            abort();          /* allocation failed */
        cache = new_cache;

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* i >= 5, 0‑based indices in cache: */
            /* a(i) = a(i-1) + 4*(a(i-3)-a(i-4)) */
            seq_t diff = cache[i - 4] - cache[i - 5];
            cache[i - 1] = cache[i - 2] + 4 * diff;
        }
        cache_size = n;
    }

    return cache[n - 1];
}
