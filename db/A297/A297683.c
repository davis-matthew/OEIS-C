/*
 * A297683  Number of 3Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 1 or 4 neighboring 1s.
 * https://oeis.org/A297683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) + 11*a(n-3) + 17*a(n-4) - 7*a(n-5) -
 *            10*a(n-6) - 18*a(n-7) - a(n-8) Initial values: a(1)= 8, a(2)=
 *            33, a(3)=150, a(4)=629, a(5)=2790, a(6)=12110, a(7)=52889,
 *            a(8)=230406. The function is memoized and works for any n≥1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for non-positive indices (should not happen). */
    if (n < 1) return 0;

    /* Static cache: array of computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_alloc = 0;   /* number of elements allocated */
    static idx_t computed = 0;      /* index up to which values are computed */

    /* Ensure the cache is large enough. */
    if (cache == NULL) {
        cache_alloc = (n > 8 ? n + 1 : 9);
        cache = (seq_t*)malloc(cache_alloc * sizeof(seq_t));
    } else if (n >= cache_alloc) {
        cache_alloc = n + 1;
        cache = (seq_t*)realloc(cache, cache_alloc * sizeof(seq_t));
    }

    /* Compute missing values using the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1)          cache[1] = 8;
        else if (i == 2)     cache[2] = 33;
        else if (i == 3)     cache[3] = 150;
        else if (i == 4)     cache[4] = 629;
        else if (i == 5)     cache[5] = 2790;
        else if (i == 6)     cache[6] = 12110;
        else if (i == 7)     cache[7] = 52889;
        else if (i == 8)     cache[8] = 230406;
        else
            cache[i] = 2 * cache[i-1] + 7 * cache[i-2] + 11 * cache[i-3] +
                       17 * cache[i-4] - 7 * cache[i-5] - 10 * cache[i-6] -
                       18 * cache[i-7] - cache[i-8];
    }
    computed = n;
    return cache[n];
}
