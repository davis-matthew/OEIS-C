/*
 * A264847  Pluritriangular numbers: a(0) = 0; a(n+1) = a(n) + the number of
 *          digits in terms a(0)..a(n).
 * https://oeis.org/A264847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0; a(1)=1; for n≥2 a(n)=2*a(n-1)-a(n-2)+digit_len(a(n-1))
 * Range:     u8 n <= 16, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper for digit count is provided by the build */
#include "../utils/digit_len.h"
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache holding terms up to the maximum index seen so far */
    static seq_t *cache  = NULL;   /* array of previous terms          */
    static idx_t   cap   = 0;      /* current capacity (≥0)            */

    /* initialize storage */
    if (cache == NULL) {
        cap   = 4;                                     /* start with 4 slots   */
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) abort();                          /* out‑of‑memory        */
        cache[0] = 0;
        cache[1] = 1;
    } else if (cap < 2) {                               /* in case capacity < 2 */
        if (cap < 1) cache[0] = 0;
        if (cap < 2) cache[1] = 1;
    }

    /* grow the cache until it can hold index n */
    while (cap <= n) {
        size_t newcap = cap ? cap << 1 : 4;            /* double size       */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();                            /* out‑of‑memory     */
        cache = tmp;
        cap   = (idx_t)newcap;
    }

    /* initialise the first two terms for safety */
    cache[0] = 0;
    cache[1] = 1;

    /* compute any missing values up to n */
    for (idx_t i = 2; i <= n; ++i) {
        /* digits of a(i-1) */
        int dk = digit_len(cache[i-1]);               /* provided helper   */
        /* recurrence: a(i) = 2*a(i-1) - a(i-2) + dk */
        cache[i] = (cache[i-1] << 1) - cache[i-2] + (seq_t)dk;
    }

    return cache[n];
}
