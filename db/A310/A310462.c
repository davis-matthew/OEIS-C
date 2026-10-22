/*
 * A310462  Coordination sequence Gal.5.138.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A310462 (coordination sequence Gal.5.138.1)
   Recurrence: a(n) = a(n-1) + a(n-9) - a(n-10)   for n > 10
   Initial values:
       a(0)=1, a(1)=4, a(2)=10, a(3)=15, a(4)=20,
       a(5)=26, a(6)=31, a(7)=36, a(8)=42, a(9)=46, a(10)=50
*/
#include <stdlib.h>

static seq_t *cache = NULL;      /* storage for computed terms */
static size_t cache_len = 0;     /* number of terms already stored (cache_len == cache[0..len-1]) */
static size_t cache_cap = 0;     /* allocated capacity */

/* ensure cache has room for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache;
#if defined(__cplusplus)
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

#if defined(__cplusplus)
    delete[] cache;
#else
    free(cache);
#endif

    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)cache_len)
        return cache[(size_t)n];

    ensure_capacity(n);

    /* compute terms that have not been stored yet */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        if (i == 0)
            cache[0] = 1;
        else if (i == 1)
            cache[1] = 4;
        else if (i == 2)
            cache[2] = 10;
        else if (i == 3)
            cache[3] = 15;
        else if (i == 4)
            cache[4] = 20;
        else if (i == 5)
            cache[5] = 26;
        else if (i == 6)
            cache[6] = 31;
        else if (i == 7)
            cache[7] = 36;
        else if (i == 8)
            cache[8] = 42;
        else if (i == 9)
            cache[9] = 46;
        else if (i == 10)
            cache[10] = 50;
        else
            cache[i] = cache[i - 1] + cache[i - 9] - cache[i - 10];
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
