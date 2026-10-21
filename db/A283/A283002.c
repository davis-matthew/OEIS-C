/*
 * A283002  Numbers that are not of the form m + (sum of base-100 digits of
 *          m).
 * https://oeis.org/A283002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   sum_base100_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Numbers that are not of the form m + (sum of base‑100 digits of m).
 * Reference: OEIS A283002.  The nth term is the nth positive integer
 * that is not equal to m + sum_base100_digits(m) for any m ≥ 1.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sum_base100_digits.h"

/* Return the nth element of the sequence (a(1) = 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for zero, return 0 */

    /* Start with a modest upper bound; will double until the nth
       allowed number is found. */
    size_t upper = (size_t)n * 2 + 100;    /* initial guess */

    for (;;) {
        /* Allocate a boolean array: 0 = allowed, 1 = forbidden */
        uint8_t *mask = (uint8_t *)calloc(upper + 1, 1);
        if (!mask) return (seq_t)0;        /* allocation failure */

        /* Mark all forbidden values ≤ upper. */
        for (idx_t m = 1; m <= (idx_t)upper; ++m)
        {
            seq_t g = (seq_t)m + sum_base100_digits(m);
            if (g <= (seq_t)upper)
                mask[(size_t)g] = 1;
        }

        /* Find the nth allowed number within this window. */
        size_t found = 0;
        for (size_t x = 1; x <= upper; ++x)
        {
            if (!mask[x])                 /* not forbidden */
            {
                ++found;
                if (found == n) {
                    free(mask);
                    return (seq_t)x;
                }
            }
        }

        free(mask);
        /* If we didn't find it, enlarge the window and try again. */
        if (upper > SIZE_MAX / 2) return (seq_t)0;   /* safety guard */
        upper *= 2;
    }
}
