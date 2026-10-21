/*
 * A266802  Coefficient of x^3 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A266802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*a(n-1) + 15*a(n-2) - 15*a(n-3) - 5*a(n-4) + a(n-5);
 *            a(0) = 2, a(1) = -12, a(2) = -16, a(3) = -294, a(4) = -1552
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial terms */
    static const seq_t init[5] = { 2, -12, -16, -294, -1552 };

    /* Return directly if index is within the initial values */
    if (n < 5) {
        return init[(size_t)n];
    }

    /* Circular buffer holding the last five computed values:
       last[0] = a(k-5), last[1] = a(k-4), ..., last[4] = a(k-1) */
    seq_t last[5];
    for (size_t i = 0; i < 5; ++i) {
        last[i] = init[i];
    }

    seq_t new_val = 0;
    for (idx_t k = 5; k <= n; ++k) {
        /* Compute a(k) from the recurrence using the buffer */
        new_val = (seq_t)5 * last[4]
                + (seq_t)15 * last[3]
                - (seq_t)15 * last[2]
                - (seq_t)5 * last[1]
                + last[0];

        /* Shift the buffer for the next iteration */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = new_val;
    }

    return new_val;
}
