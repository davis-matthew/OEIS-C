/*
 * A259737  Number of (n+2) X (3+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00010101 or 01010101.
 * https://oeis.org/A259737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence of order 10 for n>11 a(n) = 2*a(n−2) + 9*a(n−4) +
 *            4*a(n−6) – 6*a(n−8) + 4*a(n−10) Initial values: a(1)=42,
 *            a(2)=74, a(3)=190, a(4)=374, a(5)=792, a(6)=1514, a(7)=3480,
 *            a(8)=6542, a(9)=14580, a(10)=27866, a(11)=62700
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 26, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(1) is the first term in the OEIS entry.  For n < 1 the behavior
       is undefined; we simply return 0. */
    if (n < 1)
        return 0;

    /* Initial 11 terms are supplied directly.  We use a static array so
       that the values are constructed only once. */
    static const seq_t init[12] = {
        0,          /* dummy for index 0 */
        42,         /* a(1) */
        74,         /* a(2) */
        190,        /* a(3) */
        374,        /* a(4) */
        792,        /* a(5) */
        1514,       /* a(6) */
        3480,       /* a(7) */
        6542,       /* a(8) */
        14580,      /* a(9) */
        27866,      /* a(10) */
        62700       /* a(11) */
    };

    /* For small n we can answer immediately. */
    if (n <= 11)
        return init[(size_t)n];

    /* Allocate a linear array for the recurrence.  The size is n+1 because
       we index starting at 1. */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr)
        return 0;  /* memory failure – the caller can handle the zero */

    /* Fill the known terms. */
    for (idx_t i = 1; i <= 11; ++i)
        arr[i] = init[(size_t)i];

    /* Apply the recurrence for i = 12 .. n. */
    for (idx_t i = 12; i <= n; ++i) {
        arr[i] = 2*arr[i-2] + 9*arr[i-4] + 4*arr[i-6]
                 - 6*arr[i-8] + 4*arr[i-10];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
