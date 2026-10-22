/*
 * A304622  a(n) = 11 - n for 1 <= n <= 10. Thereafter a(n) = a(n-a(n-2)) +
 *          a(n-a(n-4)).
 * https://oeis.org/A304622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11 - n for 1 <= n <= 10; a(n) = a(n - a(n-2)) + a(n -
 *            a(n-4)) for n > 10 Sequence offset: 1
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* last computed index */
static idx_t cache_cap  = 0;   /* allocated capacity */

static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n < 1, but return 0 safely */
    if (n <= cache_size)
        return cache[n];
    grow_cache(n);
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        seq_t val;
        if (i <= 10)
            val = (seq_t)(11 - (idx_t)i);
        else
        {
            idx_t idx1 = i - (idx_t)cache[i - 2];
            idx_t idx2 = i - (idx_t)cache[i - 4];
            val = cache[idx1] + cache[idx2];
        }
        cache[i] = val;
    }
    cache_size = n;
    return cache[n];
}
