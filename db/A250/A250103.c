/*
 * A250103  Expansion of (1+x)/(1+x-2*x^2-3*x^3).
 * https://oeis.org/A250103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-2) + 3*a(n-3) a(0) = 1, a(1) = 0, a(2) =
 *            2
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Ensure the first three values are present. */
    if (cache_len < 3) {
        size_t old_len = cache_len;
        size_t new_len = 3;          /* we always need at least three slots */
        if (new_len > cache_len) {
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            cache = tmp;
            for (size_t i = old_len; i < new_len; ++i)
                cache[i] = 0;        /* initialize new entries */
            cache_len = new_len;
        }
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 2;
    }

    size_t target = (size_t)n;
    if (target < cache_len)
        return cache[target];

    /* Need to generate more terms up to n. */
    size_t old_len = cache_len;
    size_t new_len = target + 1;
    if (new_len < 3)
        new_len = 3;
    if (new_len > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;
        for (size_t i = old_len; i < new_len; ++i)
            cache[i] = 0;
        cache_len = new_len;
    }

    /* Compute missing terms using the recurrence. */
    size_t start = (old_len > 3) ? old_len : 3;
    for (size_t i = start; i <= target; ++i) {
        cache[i] = ((seq_t)2 * cache[i-2]) + ((seq_t)3 * cache[i-3]) - cache[i-1];
    }

    return cache[target];
}
