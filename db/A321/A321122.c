/*
 * A321122  a(n) = n-th row common denominator of A321121.
 * https://oeis.org/A321122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = LCM of denominators of w(n,k) for 0 ≤ k ≤ n (conjectured
 *            recurrence: a(n) = 52*a(n-6) - a(n-12) for n ≥ 15). Initial
 *            values a(0) … a(14) are taken from the OEIS entry
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_values[15] = {
    4,  2,  3,  8, 36, 96, 44, 360, 492, 448,
    1836, 5016, 2284, 18720, 25572
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    size_t idx = (size_t)n;

    /* Grow cache to at least idx+1 entries */
    if (cache_len <= idx)
    {
        size_t new_cap = idx + 1;
        if (new_cap > cache_cap)
        {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) /* allocation failed – abort */
            {
                /* Make the program terminate with an error code if needed */
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute new entries */
        for (size_t k = cache_len; k <= idx; ++k)
        {
            if (k < 15)
            {
                cache[k] = init_values[k];
            }
            else
            {
                /* a(k) = 52 * a(k-6) - a(k-12) */
                cache[k] = cache[k-6] * 52 - cache[k-12];
            }
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
