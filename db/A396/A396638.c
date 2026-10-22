/*
 * A396638  Number of geodesics of length n in the braid group B_3.
 * https://oeis.org/A396638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4, a(2)=12, a(3)=36, a(4)=96,
   a(n) = 3*a(n-1) - 3*a(n-3) - a(n-4) for n >= 5
   (derived from the generating function (x^4+3*x^3+x+1)/((x^2+x-1)*(x^2+2*x-1))) */

#include <stdlib.h>   /* malloc, realloc */
#include <stddef.h>   /* size_t */

static seq_t *cache = NULL;      /* array of computed values */
static size_t cache_len = 0;     /* number of allocated slots */

/* Ensure the cache contains at least n+1 entries and compute missing terms */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;

    /* initial base values are required only once */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE); /* memory failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)36;
        cache[4] = (seq_t)96;
        cache_len = 5;
    }

    /* grow if necessary */
    if (needed > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_len = needed;
    }

    /* compute terms from current length up to n */
    for (size_t i = 5; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)3 * cache[i - 1];
        term = term - (seq_t)3 * cache[i - 3];
        term = term - cache[i - 4];
        cache[i] = term;
    }
}

seq_t a(idx_t n)
{
    if (n < 5) {
        static const seq_t base[5] = {(seq_t)1, (seq_t)4, (seq_t)12,
                                      (seq_t)36, (seq_t)96};
        return base[(size_t)n];
    }
    ensure_cache(n);
    return cache[(size_t)n];
}
