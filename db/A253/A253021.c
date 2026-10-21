/*
 * A253021  Number of (n+2) X (4+2) 0..3 arrays with every consecutive three
 *          elements in every row and column not having exactly two distinct
 *          values, and in every diagonal and antidiagonal having exactly two
 *          distinct values, and new values 0 upwards introduced in row major
 *          order.
 * https://oeis.org/A253021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-3) - a(n-6) - a(n-9) for n>12, with a(1)=63,
 *            a(2)=91, a(3)=120, a(4)=155, a(5)=230, a(6)=296, a(7)=369,
 *            a(8)=546, a(9)=712, a(10)=893, a(11)=1322, a(12)=1722
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 62, u64 n <= 137, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[13] = {
    0,   /* unused, index 0 */
    63,  91, 120, 155, 230, 296,
    369, 546, 712, 893, 1322, 1722
};

seq_t a(idx_t n)
{
    /* Return known small values directly */
    if (n <= 12) {
        return base_vals[n];
    }

    /* Memoization: cache computed values up to the largest requested n */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of entries currently stored (max index) */

    /* Grow cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed: fall back to computing on the fly */
            seq_t res = 0;
            for (idx_t i = cache_cap + 1; i <= n; ++i) {
                if (i <= 12) {
                    res = base_vals[i];
                } else {
                    res = new_cache[i-3] * 3 - new_cache[i-6] - new_cache[i-9];
                }
            }
            return res;
        }
        cache = new_cache;
        /* Compute new values up to n */
        for (idx_t i = cache_cap + 1; i <= new_cap; ++i) {
            if (i <= 12) {
                cache[i] = base_vals[i];
            } else {
                cache[i] = cache[i-3] * 3 - cache[i-6] - cache[i-9];
            }
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
