/*
 * A360645  Number of 4-dimensional tilings of a 2 X 2 X 2 X n box with 2 X 2
 *          X 1 X 1 plates.
 * https://oeis.org/A360645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 15*a(n-2) - 18*a(n-3) with a(0)=1, a(1)=3,
 *            a(2)=30
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)30;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size of cache */
    static idx_t cache_len = 0;    /* next free index in cache */

    /* Grow cache if needed. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (n + 16) * 2;   /* ample extra room */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure – unlikely in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Ensure the first three values are present in the cache. */
    if (cache_len < 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)30;
        cache_len = 3;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        /* 5*a1 + 15*a2 - 18*a3 */
        seq_t val = 5 * a1 + 15 * a2;
        val = val - 18 * a3;
        cache[i] = val;
    }

    cache_len = (idx_t)(n + 1);
    return cache[n];
}
