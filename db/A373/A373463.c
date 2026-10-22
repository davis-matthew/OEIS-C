/*
 * A373463  Expansion of 1/((1 + x)^5 - 2*x^5).
 * https://oeis.org/A373463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -5*a(n-1) - 10*a(n-2) - 10*a(n-3) - 5*a(n-4) + a(n-5).
 *            Initial values: a(0)=1, a(1)=-5, a(2)=15, a(3)=-35, a(4)=70
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 28, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Initial values for n = 0 … 4 */
    static const seq_t init[5] = { 1, -5, 15, -35, 70 };
    if (n < 5) {
        return init[n];
    }

    /* Circular buffer of the last five values */
    seq_t buf[5] = { init[0], init[1], init[2], init[3], init[4] };

    for (idx_t i = 5; i <= n; ++i) {
        seq_t newv =
              (seq_t)(-5) * buf[(i - 1) % 5]
            + (seq_t)(-10) * buf[(i - 2) % 5]
            + (seq_t)(-10) * buf[(i - 3) % 5]
            + (seq_t)(-5) * buf[(i - 4) % 5]
            +        buf[(i - 5) % 5];
        buf[i % 5] = newv;
    }

    return buf[n % 5];
}
