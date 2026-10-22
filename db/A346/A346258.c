/*
 * A346258  E.g.f.: exp(x) / (1 - 3 * x)^(1/3).
 * https://oeis.org/A346258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A346258
   Recurrence: a(0)=1, a(1)=2,
   for n ≥ 0:   a(n+2) = (3*n+5)*a(n+1) – 3*(n+1)*a(n)
   This function memoises the values so that calls with arbitrary
   ordering of n are fast.  It uses dynamic allocation via the
   standard library (malloc/realloc) and works for any seq_t
   supported by the build system. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;  /* number of terms currently stored   */
    static size_t cache_cap  = 0;  /* capacity of allocated array         */

    /* Initialise cache with the first two terms if needed. */
    if (cache == NULL) {
        cache_cap  = 2;
        cache_size = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) { /* Out of memory – behaviour unspecified. */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)2;   /* a(1) */
    }

    /* Compute terms up to n if they are not already cached. */
    if ((size_t)n + 1 > cache_size) {
        /* Grow the array if necessary. */
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 2;
            while (new_cap < needed)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) { /* Out of memory – behaviour unspecified. */
                return (seq_t)0;
            }
            cache      = tmp;
            cache_cap  = new_cap;
        }

        /* Recurrence: a[i] = (3*(i-2)+5)*a[i-1] - 3*(i-1)*a[i-2]  */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t a_prev  = cache[i - 1];
            seq_t a_prev2 = cache[i - 2];
            seq_t coeff1  = (seq_t)(3 * (i - 2) + 5);
            seq_t coeff2  = (seq_t)(3 * (i - 1));
            cache[i] = coeff1 * a_prev - coeff2 * a_prev2;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
