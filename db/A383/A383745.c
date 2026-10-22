/*
 * A383745  Numbers k of the form x*(x+1) whose sum of digits is of the form
 *          y*(y+1).
 * https://oeis.org/A383745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 94, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A383745:  k = x*(x+1) with digit_sum(k) = y*(y+1)  */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached sequence values          */
    static size_t len   = 0;      /* number of cached terms           */
    static size_t cap   = 0;      /* allocation capacity of cache     */
    static seq_t next_x = 0;      /* next x to test (0‑based)          */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* generate terms until we have at least n terms */
    while (len < n) {
        seq_t x = next_x;
        seq_t k = x * (x + 1);          /* k = x*(x+1)                  */
        seq_t sum = digit_sum(k);       /* sum of decimal digits of k   */

        /* check whether sum is of the form y*(y+1)   */
        /* i.e. 1 + 4*sum must be a perfect square   */
        seq_t D = sum * 4 + 1;
        seq_t sqrtD = floor_sqrt(D);
        if (sqrtD * sqrtD == D) {
            /* store k in cache, growing if necessary */
            if (len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[len++] = k;
        }
        next_x++;                         /* move to next x              */
    }

    return cache[n - 1];
}
