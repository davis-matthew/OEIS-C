/*
 * A254627  Indices of centered pentagonal numbers (A005891) that are also
 *          triangular numbers (A000217).
 * https://oeis.org/A254627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A254627
 *  Formula: a(n) = a(n-1) + 18*a(n-2) - 18*a(n-3) - a(n-4) + a(n-5),
 *  with initial terms a(1)=1, a(2)=2, a(3)=11, a(4)=28, a(5)=189.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return for indices below the offset (n==0) is undefined, we return 0. */
    if (n == 0)
        return 0;

    /* Handle the first five terms directly. */
    switch (n) {
    case 1: return 1;
    case 2: return 2;
    case 3: return 11;
    case 4: return 28;
    case 5: return 189;
    }

    /* Use a sliding window of the last five values to compute the recurrence. */
    seq_t v[5] = {1, 2, 11, 28, 189};   /* v[0]=a(n-5), ..., v[4]=a(n-1) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t new_val = v[4] + (seq_t)18 * v[3]; /* a(n-1) + 18*a(n-2) */
        new_val -= (seq_t)18 * v[2];            /* -18*a(n-3)          */
        new_val -= v[1];                       /* -a(n-4)             */
        new_val += v[0];                       /* +a(n-5)             */

        /* Shift the window: discard the oldest, add the newest. */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = new_val;
    }

    return v[4]; /* a(n) */
}
