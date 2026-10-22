/*
 * A400478  a(1)=1, a(2)=2; thereafter a(n) = (a(n-1)-a(n-2))*sum of
 *          digits(a(n-1)+a(n-2)).
 * https://oeis.org/A400478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, new
 * Formula:   a(n) = (a(n-1)-a(n-2))*sumDigits(a(n-1)+a(n-2)), a(1)=1, a(2)=2
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 12, u64 n <= 18, u128 n <= 27,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

/* Helper to compute the sum of decimal digits of a seq_t value. */

/* Static cache for memoization */
static seq_t *cache   = NULL;   /* array of computed values */
static size_t  size   = 0;      /* number of values in cache (entries a(1)…a(size)) */
static size_t  capacity = 0;    /* allocated capacity of cache */

/* Ensure cache has room for at least n elements (a(1)…a(n)). */
static void ensure_capacity(idx_t n)
{
    if ((idx_t)capacity >= n) return;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!tmp) abort();           /* out of memory */
    cache = tmp;
    capacity = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n<1, but harmless */

    ensure_capacity(n);

    /* Compute any missing terms up to n. */
    for (idx_t i = (idx_t)size + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 1;
        } else if (i == 2) {
            val = 2;
        } else {
            /* a(i-1) is at cache[i-2], a(i-2) at cache[i-3] */
            seq_t prev     = cache[i - 2];
            seq_t prevprev = cache[i - 3];
            seq_t sum_ab   = prev + prevprev;
            seq_t sumdig   = digit_sum(sum_ab);
            seq_t diff     = prev - prevprev;
            val = diff * sumdig;
        }
        cache[i - 1] = val;
        size = (size_t)i;
    }

    return cache[n - 1];
}
