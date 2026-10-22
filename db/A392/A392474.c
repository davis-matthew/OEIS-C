/*
 * A392474  Sum of the largest parts in the partitions of n into 3 parts whose
 *          smallest part is even.
 * https://oeis.org/A392474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + 2*a(n-6) - 2*a(n-7) * -
 *            2*a(n-8) + 2*a(n-9) - a(n-12) + a(n-13) + a(n-14) - a(n-15) *
 *            Initial values (for n = 0 … 15): *
 *            0,0,0,0,0,0,2,3,7,9,15,18,30,35,51,58
 * Range:     u8 n <= 23, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[16] = {
    0, 0, 0, 0, 0, 0,
    2, 3, 7, 9, 15, 18,
    30, 35, 51, 58
};

/* Cached values for a(n).  cachelen is the number of usable elements. */
static seq_t *cache = NULL;
static idx_t cachelen = 0;

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (n + 1 > cachelen) {
        size_t newlen = (size_t)(n + 1);

        if (cache == NULL) {
            cache = (seq_t *)malloc(newlen * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        }

        /* Initialize the first 16 values if they have not been set yet */
        size_t start = cachelen;
        for (size_t i = start; i <= 15 && i < newlen; ++i) {
            cache[i] = init_vals[i];
        }
        cachelen = newlen;

        /* Compute new entries using the recurrence */
        for (size_t i = start; i < newlen; ++i) {
            /* Only use the recurrence for i >= 16; otherwise values are
             * already taken from init_vals. */
            if (i >= 16) {
                cache[i] =
                    cache[i - 1]          /* a(i-1) */
                  + cache[i - 2]          /* a(i-2) */
                  - cache[i - 3]          /* a(i-3) */
                  + (cache[i - 6] << 1)  /* 2*a(i-6) */
                  - (cache[i - 7] << 1)  /* 2*a(i-7) */
                  - (cache[i - 8] << 1)  /* 2*a(i-8) */
                  + (cache[i - 9] << 1)  /* 2*a(i-9) */
                  - cache[i - 12]        /* -a(i-12) */
                  + cache[i - 13]       /* +a(i-13) */
                  + cache[i - 14]       /* +a(i-14) */
                  - cache[i - 15];      /* -a(i-15) */
            }
        }
    }

    return cache[(size_t)n];
}
