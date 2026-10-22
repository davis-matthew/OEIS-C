/*
 * A301438  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 6 horizontally, vertically or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A301438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 4*a(n-2) - 5*a(n-3) with a(1)=4, a(2)=32,
 *            a(3)=232
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;
    idx_t new_len = n;
    /* reallocate cache */
    cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    for (idx_t i = cache_len; i < new_len; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)4;
        else if (i == 1)
            cache[i] = (seq_t)32;
        else if (i == 2)
            cache[i] = (seq_t)232;
        else
        {
            cache[i] = ((seq_t)8 * cache[i - 1])
                       - ((seq_t)4 * cache[i - 2])
                       - ((seq_t)5 * cache[i - 3]);
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* offset is 1; value below the offset is irrelevant */
    ensure_cache(n);
    return cache[n - 1];
}
