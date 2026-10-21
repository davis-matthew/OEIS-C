/*
 * A266700  Coefficient of x in minimal polynomial of the continued fraction
 *          [1^n,1/2,1,1,1,...], where 1^n means n ones.
 * https://oeis.org/A266700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) with initial values a(0)=0,
 *            a(1)=-10, a(2)=-12
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 21, i64 n <= 44, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* ensure cache holds values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    idx_t needed = n + 1;
    if (needed < 3) needed = 3;  /* at least three entries for the base case */

    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)-10;
        cache[2] = (seq_t)-12;
        cache_size = needed;
        /* if needed > 3, we will compute remaining entries below */
    } else if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = needed;
    }

    /* compute any missing values */
    if (cache_size <= 3) return;
    const seq_t two = (seq_t)2;
    for (idx_t i = (cache_size == needed ? 3 : cache_size); i <= n; ++i) {
        cache[i] = two * cache[i-1] + two * cache[i-2] - cache[i-3];
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
