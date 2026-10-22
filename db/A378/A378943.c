/*
 * A378943  Numbers obtained from the tribonacci triangle formed by the number
 *          of connection points in the paths obtained by Pell walk on the
 *          square grid.
 * https://oeis.org/A378943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2, a(2)=3, a(3)=7, a(4)=13, a(5)=25, and for n≥6 * a(n) =
 *            a(n-1) + 2·a(n-2) – a(n-4) – a(n-5)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises computed terms so it works for any call order. */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms, 1‑based index in OEIS sense */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* how many terms are in cache */

    /* Initialise cache with first five terms if needed */
    if (cache_len == 0) {
        cache_len = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) abort();
        cache[0] = (seq_t)2;   /* a(1) */
        cache[1] = (seq_t)3;   /* a(2) */
        cache[2] = (seq_t)7;   /* a(3) */
        cache[3] = (seq_t)13;  /* a(4) */
        cache[4] = (seq_t)25;  /* a(5) */
    }

    /* If requested term is already cached, return it immediately */
    if (n <= cache_len)
        return cache[n - 1];

    /* Need to compute new terms up to n */
    idx_t old_len = cache_len;   /* remember current length before grow */

    /* Grow the cache to hold terms up to n */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
    if (tmp == NULL) abort();
    cache = tmp;
    cache_len = n;               /* now cache has room for 1..n */

    /* Compute the missing terms starting from old_len+1 */
    for (idx_t i = old_len + 1; i <= n; ++i) {
        /* a(i-1) = cache[i-2], a(i-2) = cache[i-3], a(i-4) = cache[i-5], a(i-5) = cache[i-6] */
        seq_t a_im1 = cache[i - 2];
        seq_t a_im2 = cache[i - 3];
        seq_t a_im4 = cache[i - 5];
        seq_t a_im5 = cache[i - 6];

        cache[i - 1] = a_im1 + ((seq_t)2 * a_im2) - a_im4 - a_im5;
    }

    return cache[n - 1];
}
