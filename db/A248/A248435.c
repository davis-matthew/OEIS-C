/*
 * A248435  Number of length 2+2 0..n arrays with every three consecutive
 *          terms having the sum of some two elements equal to twice the
 *          third.
 * https://oeis.org/A248435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-5) - a(n-7) + a(n-8) initial
 *            values: a(1) = 2, a(2) = 9, a(3) = 20, a(4) = 45, a(5) = 70,
 *            a(6) = 105, a(7) = 140, a(8) = 189
 * Range:     u8 n <= 9, u16 n <= 144, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for computed terms */
static seq_t *cache = NULL;
static idx_t cache_last = 0;      /* highest n that has been computed */
static size_t cache_cap = 0;      /* size of allocated array */

/* Grow the cache to at least size 'needed' (indices 0..needed) */
static void cache_grow(idx_t needed)
{
    if (cache_cap > (size_t)needed)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= (size_t)needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    if (n > cache_last) {
        cache_grow(n);
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            size_t ii = (size_t)i;   /* index in the array */
            if (i == 1)      cache[ii] = (seq_t)2;
            else if (i == 2) cache[ii] = (seq_t)9;
            else if (i == 3) cache[ii] = (seq_t)20;
            else if (i == 4) cache[ii] = (seq_t)45;
            else if (i == 5) cache[ii] = (seq_t)70;
            else if (i == 6) cache[ii] = (seq_t)105;
            else if (i == 7) cache[ii] = (seq_t)140;
            else if (i == 8) cache[ii] = (seq_t)189;
            else
                cache[ii] = cache[ii-1] + cache[ii-3]
                                   - cache[ii-5] - cache[ii-7]
                                   + cache[ii-8];
        }
        cache_last = n;
    }
    return cache[(size_t)n];
}
