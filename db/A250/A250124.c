/*
 * A250124  Coordination sequence of point of type 3.3.12.4 in 4-uniform
 *          tiling {3.3.4.3.4; 3.3.4.12; 3.3.12.4; 3.4.3.12}.
 * https://oeis.org/A250124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-5) + a(n-7) - a(n-12) for n ≥ 15 with initial terms
 *            a(0)=1, a(1)=4, a(2)=7, a(3)=10, a(4)=15, a(5)=16, a(6)=21,
 *            a(7)=29, a(8)=28, a(9)=34, a(10)=33, a(11)=40, a(12)=48,
 *            a(13)=45, a(14)=53 (indices follow OEIS convention). The
 *            recurrence comes from the rational generating function of th...
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[15] = {
    1, 4, 7, 10, 15, 16, 21, 29, 28, 34, 33, 40, 48, 45, 53
};

static seq_t *cache = NULL;     /* dynamic cache of computed values   */
static idx_t cache_size = 0;    /* how many values are stored (size) */

seq_t a(idx_t n)
{
    /* return a known value for the initial range directly */
    if (n < 15)
        return init_vals[n];

    /* first time we need any terms beyond the first 15 in the cache,
       initialise the cache with the first 15 sequence values     */
    if (cache == NULL) {
        cache_size = 15;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL)       /* allocation failure → return 0 */
            return (seq_t)0;
        for (idx_t i = 0; i < 15; ++i)
            cache[i] = init_vals[i];
    }

    /* if the requested index is already computed, return it */
    if (n < cache_size)
        return cache[n];

    /* enlarge the cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL)       /* allocation failure → return 0 */
            return (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* compute all values up to n using the recurrence
       a(i) = a(i-5) + a(i-7) - a(i-12) for i ≥ 15                 */
    for (idx_t i = 15; i <= n; ++i)
        cache[i] = cache[i - 5] + cache[i - 7] - cache[i - 12];

    return cache[n];
}
