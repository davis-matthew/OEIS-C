/*
 * A377919  a(0) = 0; thereafter a(n) is the lexicographically earliest
 *          missing nonnegative integer such that the digits in the sequence
 *          (ignoring the commas) alternate in parity.
 * https://oeis.org/A377919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-4)*100 + d where d = 10 if n%4==2 or 3, otherwise d =
 *            1. Initial values: a(0)=0, a(1)=1, a(2)=2, a(3)=10, a(4)=101,
 *            a(5)=21, a(6)=210
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements stored */
    static size_t cache_cap  = 0;   /* capacity of allocated array */

    /* initialise cache on first entry */
    if (cache == NULL)
    {
        cache_cap = 7;                          /* store initial 7 values */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* out‑of‑memory: return 0 as safety */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)101;
        cache[5] = (seq_t)21;
        cache[6] = (seq_t)210;
        cache_size = 7;
    }

    /* returned value already stored? */
    if ((size_t)n < cache_size)
        return cache[n];

    /* extend capacity if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap)
    {
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* out‑of‑memory: fallback */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute new entries up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        if (i < 7) /* safety: already stored */
            continue;

        seq_t prev = cache[i - 4];
        seq_t digit = ((i % 4 == 0) || (i % 4 == 1)) ? (seq_t)1 : (seq_t)10;
        cache[i] = prev * (seq_t)100 + digit;
    }

    cache_size = needed;
    return cache[n];
}
