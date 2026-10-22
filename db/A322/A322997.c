/*
 * A322997  Number of iterations of A049820(x) = x - A000005(x) needed to
 *          reach an odd number or zero, when starting from x = 2n.
 * https://oeis.org/A322997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 293, u16 n <= 9824, u32 n <= 9824, u64 n <= 9824,
 *            u128 n <= 9824, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A322997: Number of iterations of A049820(x) = x - d(x)
 * (where d(x) is the divisor count A000005) needed to reach an odd
 * number or zero, starting from x = 2n.
 * The function returns the exact value for any n >= 0.
 */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---- static cache for divisor counts up to the maximum 2n seen ---- */
    static size_t div_cap = 0;   /* current array size */
    static int   *div_cache = NULL;  /* divisor counts, index is value */

    /* ensure cache covers at least 2n */
    idx_t max_needed = n * 2;
    if ((size_t)max_needed + 1 > div_cap) {
        size_t new_cap = (size_t)max_needed + 1;
        int *new_arr = (int *)malloc(sizeof(int) * new_cap);
        for (size_t i = 0; i < new_cap; ++i) new_arr[i] = 0;
        for (size_t i = 1; i < new_cap; ++i)
            for (size_t j = i; j < new_cap; j += i)
                ++new_arr[j];
        free(div_cache);
        div_cache = new_arr;
        div_cap   = new_cap;
    }

    /* ---- main iteration loop ---- */
    seq_t x      = (seq_t)(n * 2);   /* start value */
    seq_t count  = 0;                /* number of iterations */

    while (1) {
        if (x == 0) break;              /* reached zero */
        if ((x & 1) != 0) break;        /* reached an odd number */

        /* divisor count of the current x */
        idx_t xi = (idx_t)x;            /* x is small enough to cast safely */
        seq_t d = (seq_t)div_cache[xi];

        x -= d;                         /* next value of the iteration */
        ++count;
    }

    return count;
}
