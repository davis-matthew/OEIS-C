/*
 * A269188  Number of n X 2 0..3 arrays with some element plus some
 *          horizontally, diagonally, antidiagonally or vertically adjacent
 *          neighbor totalling three no more than once.
 * https://oeis.org/A269188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=16, a(2)=108, a(3)=708, a(4)=4476, a(5)=27684
 *            a(n)=10*a(n-1)-21*a(n-2)-20*a(n-3)-4*a(n-4) for n>5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;   /* array of cached terms */
static size_t cache_size = 0; /* number of terms that have been computed  */
static size_t cache_cap  = 0; /* current capacity of the array            */

/* Compute a(n) for n>=1.  The function is memoised; calls may come in
   any order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* unused, but harmless            */

    /* initialise cache with known first 5 terms */
    if (!cache) {
        grow_seq(&cache, &cache_cap, 5);   /* ensure room for at least 5 values */
        cache[0] = (seq_t)16;    /* a(1) */
        cache[1] = (seq_t)108;   /* a(2) */
        cache[2] = (seq_t)708;   /* a(3) */
        cache[3] = (seq_t)4476;  /* a(4) */
        cache[4] = (seq_t)27684; /* a(5) */
        cache_size = 5;
    }

    /* Return immediately if requested value is already cached */
    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    /* Extend cache to hold up to n terms */
    size_t prev_cap = cache_size;
    if ((size_t)n > cache_cap)
        grow_seq(&cache, &cache_cap, (size_t)n);

    /* Compute missing terms */
    for (size_t i = prev_cap; i < (size_t)n; ++i) {
        /* i corresponds to term a(i+1).  The recurrence requires
           a(i) with i>=5 (i.e. a(6) and beyond).  For i<5 the base
           terms are already stored. */
        if (i < 5) continue;     /* base terms are already set */
        cache[i] = (seq_t)10 * cache[i-1]
                 - (seq_t)21 * cache[i-2]
                 - (seq_t)20 * cache[i-3]
                 - (seq_t) 4 * cache[i-4];
    }

    cache_size = (size_t)n;
    return cache[(size_t)n - 1];
}
