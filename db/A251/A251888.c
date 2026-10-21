/*
 * A251888  Number of (n+2) X (2+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not 2 3 6 or 7 and every diagonal and antidiagonal
 *          sum 2 3 6 or 7.
 * https://oeis.org/A251888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 40, u64 n <= 88, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + 6*a(n-3) - 6*a(n-4) - 8*a(n-6) + 8*a(n-7)   (n>14)
 * Initial values (OEIS offset 1):
 *   a(1)=1664, a(2)=1232, a(3)=602, a(4)=530, a(5)=594, a(6)=790,
 *   a(7)=1182, a(8)=1670, a(9)=2474, a(10)=3890, a(11)=5642,
 *   a(12)=8594, a(13)=13922, a(14)=20498
 * This function caches computed values so subsequent calls are O(1) on already
 * visited indices.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/*                 Static cache for computed values              */
/* ------------------------------------------------------------ */
static seq_t *cache = NULL;      /* allocated array, 0..max_index (0 unused)   */
static size_t cache_size = 0;    /* number of allocated slots (>=1)            */

/* ------------------------------------------------------------ */
/*                Ensure cache has room for index n             */
/* ------------------------------------------------------------ */
static void ensure_cache(size_t n)
{
    /* Need storage for indices 0..n */
    size_t required = n + 1;
    if (cache_size >= required)
        return;

    /* Grow: double until big enough, or start from 16 */
    size_t newsize = cache_size ? cache_size * 2 : 16;
    while (newsize < required)
        newsize <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp)
        abort();                 /* unrecoverable memory allocation failure */

    /* Zero the newly allocated part (old data remains unchanged) */
    if (newsize > cache_size)
        for (size_t i = cache_size; i < newsize; ++i)
            tmp[i] = (seq_t)0;

    cache = tmp;
    cache_size = newsize;
}

/* ------------------------------------------------------------ */
/*                     Initial values generator                */
/* ------------------------------------------------------------ */
static void init_base(void)
{
    /* Ensure space for first 14 entries         */
    ensure_cache(14);

    /* Only set them once */
    static const seq_t init[14] = {
        1664u, 1232u, 602u, 530u, 594u, 790u,
        1182u, 1670u, 2474u, 3890u, 5642u,
        8594u, 13922u, 20498u
    };
    for (size_t i = 1; i <= 14; ++i)
        cache[i] = init[i - 1];
}

/* ------------------------------------------------------------ */
/*                      Public function a(n)                    */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but harmless for n=0 */

    size_t idx = (size_t)n;
    ensure_cache(idx);

    /* Initialize base values once */
    if (idx <= 14 && cache[1] == (seq_t)0)
        init_base();

    /* For n <= 14 we already have the value */
    if (idx <= 14)
        return cache[idx];

    /* For n > 14 compute up to idx, avoiding repeated calculation
       of indices already stored in cache. */
    for (size_t i = 15; i <= idx; ++i) {
        seq_t term = cache[i - 1];
        term += 6 * cache[i - 3];
        term += 8 * cache[i - 7];
        term -= 6 * cache[i - 4];
        term -= 8 * cache[i - 6];
        cache[i] = term;
    }
    return cache[idx];
}
