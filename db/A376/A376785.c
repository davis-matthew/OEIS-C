/*
 * A376785  Expansion of (1 + x - x^3)/((1 + x - x^3)^2 - 4*x).
 * https://oeis.org/A376785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) + 2*a(n-4) - a(n-6) with
 *            initial values a(0)=1, a(1)=3, a(2)=5, a(3)=8, a(4)=19, a(5)=46
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>   /* provided if needed */

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* expand the cache to at least size n+1 and fill new entries */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    idx_t const new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!cache) {
        /* memory allocation failed – abort with zero */
        exit(1);
    }

    /* initialise new terms */
    for (idx_t i = cache_len; i < new_len; ++i) {
        if (i == 0) cache[i] = (seq_t)1;
        else if (i == 1) cache[i] = (seq_t)3;
        else if (i == 2) cache[i] = (seq_t)5;
        else if (i == 3) cache[i] = (seq_t)8;
        else if (i == 4) cache[i] = (seq_t)19;
        else if (i == 5) cache[i] = (seq_t)46;
        else
            cache[i] = (seq_t)2 * cache[i-1]
                     - cache[i-2]
                     + (seq_t)2 * cache[i-3]
                     + (seq_t)2 * cache[i-4]
                     - cache[i-6];
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
