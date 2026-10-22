/*
 * A317513  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4, 5, 6 or 7 king-move adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A317513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 6*a(n-2) + 64*a(n-3) Initial values: a(1)=8,
 *            a(2)=128, a(3)=2032
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* cache of a(1), a(2), ... stored at cache[0], cache[1], ... */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Ensure the cache holds indices 0 .. n-1.  Return true on success. */
static bool _ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return true;

    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        return false;
    cache = tmp;

    for (idx_t i = cache_len; i < n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)8;
        } else if (i == 1) {
            cache[1] = (seq_t)128;
        } else if (i == 2) {
            cache[2] = (seq_t)2032;
        } else {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t t = a3 * (seq_t)64;   /* 64*a(n-3)   */
            t += a1 * (seq_t)16;        /* +16*a(n-1)  */
            t -= a2 * (seq_t)6;         /* -6*a(n-2)   */
            cache[i] = t;
        }
    }
    cache_len = new_len;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* behavior below offset is arbitrary */
        return (seq_t)0;
    _ensure_cache(n);
    return cache[n - 1];        /* a(1) is stored at cache[0] */
}
