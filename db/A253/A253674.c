/*
 * A253674  Indices of centered octagonal numbers (A016754) which are also
 *          centered triangular numbers (A005448).
 * https://oeis.org/A253674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 98*a(n-2) - 98*a(n-3) - a(n-4) + a(n-5) Initial
 *            values: a(1) = 1 a(2) = 10 a(3) = 40 a(4) = 931 a(5) = 3871
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise only once */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0;              /* allocation failed – return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)40;
        cache[3] = (seq_t)931;
        cache[4] = (seq_t)3871;
    }

    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!new_cache) return 0;          /* allocation failed – return 0 */
        cache = new_cache;
        /* compute new terms using the recurrence */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            cache[k-1] = cache[k-2]                 /* a(k-1)  */
                       + (seq_t)98 * cache[k-3]     /* 98*a(k-2) */
                       - (seq_t)98 * cache[k-4]     /* -98*a(k-3) */
                       - cache[k-5]                 /* -a(k-4)  */
                       + cache[k-6];                /* +a(k-5)  */
        }
        cache_size = n;
    }

    return cache[n-1];
}
