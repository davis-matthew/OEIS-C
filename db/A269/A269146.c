/*
 * A269146  Number of n X 2 0..3 arrays with some element plus some
 *          horizontally, antidiagonally or vertically adjacent neighbor
 *          totalling three exactly once.
 * https://oeis.org/A269146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 38*a(n-2) + 12*a(n-3) - a(n-4) for n>5; *
 *            a(1)=4, a(2)=80, a(3)=768, a(4)=6224, a(5)=46464
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* 1‑based index; cache[0] unused          */
static idx_t cache_size = 0;       /* current allocated capacity (elements)  */

static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;
    idx_t new_size = cache_size ? cache_size << 1 : 16;
    while (new_size <= n)
        new_size <<= 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    /* Newly allocated entries are uninitialised; we will set them lazily. */
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset <1, but safe */

    ensure_cache(n);

    /* Initialise the minimal recurrence seed once */
    if (cache[1] == 0) {
        cache[1] = (seq_t)4;        /* a(1) */
        cache[2] = (seq_t)80;       /* a(2) */
        cache[3] = (seq_t)768;      /* a(3) */
        cache[4] = (seq_t)6224;     /* a(4) */
        cache[5] = (seq_t)46464;    /* a(5) */
    }

    /* Compute further terms using the linear recurrence */
    for (idx_t i = 6; i <= n; ++i) {
        if (cache[i] == 0) {
            seq_t t = 12 * cache[i-1];      /* 12*a(n-1) */
            t -= 38 * cache[i-2];           /* -38*a(n-2) */
            t += 12 * cache[i-3];           /* +12*a(n-3) */
            t -=    cache[i-4];             /* -a(n-4)   */
            cache[i] = t;
        }
    }

    return cache[n];
}
