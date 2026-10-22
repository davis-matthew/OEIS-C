/*
 * A378817  Hankel sequence transform of A378816.
 * https://oeis.org/A378817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-2) - 3*a(n-4) + 2*a(n-6) - a(n-8) Initial values:
 *            a(0) = -1, a(1) = -5, a(2) = 6, a(3) = -10, a(4) = 11, a(5) =
 *            -1, a(6) = 1, a(7) = 17
 * Range:     i8 n <= 62, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0..7 */
    static const seq_t base_vals[8] =
    {
        (seq_t)-1,  /* a(0) */
        (seq_t)-5,  /* a(1) */
        (seq_t)6,   /* a(2) */
        (seq_t)-10, /* a(3) */
        (seq_t)11,  /* a(4) */
        (seq_t)-1,  /* a(5) */
        (seq_t)1,   /* a(6) */
        (seq_t)17   /* a(7) */
    };

    if (n < 8)
        return base_vals[n];

    /* Allocate array to hold values up to n */
    size_t len = (size_t)n + 1;
    seq_t *arr = (seq_t*)malloc(len * sizeof(seq_t));
    if (!arr) { /* allocation failed – return 0 as safe fallback */
        return (seq_t)0;
    }

    /* Copy base values */
    for (idx_t i = 0; i < 8; ++i)
        arr[i] = base_vals[i];

    /* Compute subsequent terms using the recurrence */
    for (idx_t i = 8; i <= n; ++i) {
        arr[i] = (arr[i - 2] * 2)
               - (arr[i - 4] * 3)
               + (arr[i - 6] * 2)
               - arr[i - 8];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
