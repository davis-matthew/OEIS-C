/*
 * A375984  Number of subsets of {1,2,...,n} such that no two elements differ
 *          by 3, 4, or 5.
 * https://oeis.org/A375984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-6) + a(n-7) + 2*a(n-8) for n >= 8 with
 *            initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=12,
 *            a(5)=16, a(6)=20, a(7)=25
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 0;

    /* Initialize cache with first 8 terms on first call */
    if (cache == NULL) {
        cache_size = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;           /* allocation failure -> 0 */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 4;
        cache[3] = 8;
        cache[4] = 12;
        cache[5] = 16;
        cache[6] = 20;
        cache[7] = 25;
        computed = 7;
    }

    /* If requested index already in cache, return it */
    if (n <= computed)
        return cache[n];

    /* Grow cache to at least n+1 elements */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n) {
            new_size <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) {                     /* allocation failure -> 0 */
            free(cache);
            cache = NULL;
            cache_size = 0;
            computed = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i < 8) {
            /* Should not occur because initial terms already set */
            continue;
        }
        /* a(i) = a(i-1) + a(i-6) + a(i-7) + 2*a(i-8) */
        seq_t val = cache[i - 1] + cache[i - 6] + cache[i - 7] + (cache[i - 8] << 1);
        cache[i] = val;
    }
    computed = n;
    return cache[n];
}
