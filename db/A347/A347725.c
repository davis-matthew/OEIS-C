/*
 * A347725  Number of irredundant sets in the (2n-1)-triangular snake graph
 *          (for n > 1).
 * https://oeis.org/A347725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+a(n-2)+a(n-3) for n > 3; Initial values: a(1)=1,
 *            a(2)=4, a(3)=10
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation across calls */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_max = 0;

    /* Handle the first three terms directly */
    if (n <= 3) {
        if (n == 1) return (seq_t)1;
        if (n == 2) return (seq_t)4;
        return (seq_t)10;            /* n == 3 */
    }

    /* Ensure cache covers up to n */
    if (n > cached_max) {
        /* Resize cache if necessary */
        size_t need = (size_t)n + 1;          /* index n must be reachable */
        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < need) new_cap = need;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);                /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise base cases if not already done */
        if (cached_max < 1) { cache[1] = (seq_t)1; cached_max = 1; }
        if (cached_max < 2) { cache[2] = (seq_t)4; cached_max = 2; }
        if (cached_max < 3) { cache[3] = (seq_t)10; cached_max = 3; }

        /* Compute missing values up to n */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i-1] + cache[i-2] + cache[i-3];
        }
        cached_max = n;
    }

    return cache[n];
}
