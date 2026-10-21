/*
 * A266806  Coefficient of x^2 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(6),1,1,...], where 1^n means n ones. S.
 * https://oeis.org/A266806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*a(n-1) + 15*a(n-2) - 15*a(n-3) - 5*a(n-4) + a(n-5),
 *            with a(0) = -13, a(1) = 143, a(2) = 527, a(3) = 4859, a(4) =
 *            30119
 * Range:     i8 n <= 0, i16 n <= 4, i32 n <= 9, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    if (n == 0) return (seq_t)-13;
    if (n == 1) return (seq_t)143;
    if (n == 2) return (seq_t)527;
    if (n == 3) return (seq_t)4859;
    if (n == 4) return (seq_t)30119;

    /* Sliding window for the last five values */
    seq_t v0 = (seq_t)-13;  /* a(n-5) */
    seq_t v1 = (seq_t)143;  /* a(n-4) */
    seq_t v2 = (seq_t)527;  /* a(n-3) */
    seq_t v3 = (seq_t)4859; /* a(n-2) */
    seq_t v4 = (seq_t)30119;/* a(n-1) */

    seq_t next = 0;
    /* Iterate from 5 up to n */
    for (idx_t i = 5; i <= n; ++i) {
        next = (seq_t)5 * v4
             + (seq_t)15 * v3
             - (seq_t)15 * v2
             - (seq_t)5  * v1
             + v0;
        /* Shift the window */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = next;
    }
    return next;
}
