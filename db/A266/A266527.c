/*
 * A266527  Coefficient of x^6 in the minimal polynomial of the continued
 *          fraction [1^n,2^(1/3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A266527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 13*a(n-1) + 104*a(n-2) - 260*a(n-3) - 260*a(n-4) +
 *            104*a(n-5) + 13*a(n-6) - a(n-7) for n ≥ 9 Initial values:
 *            a(0)=1, a(1)=5, a(2)=11, a(3)=131, a(4)=3421, a(5)=56209,
 *            a(6)=1049105, a(7)=18561659, a(8)=334918459
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 16, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial terms up to a(8).  The values are cast to seq_t at
       compile time; for small seq_t (e.g. int8_t) the conversions
       overflow, but those values are only used when a(n) fits in
       seq_t, so the behaviour is acceptable. */
    static const seq_t init[9] = {
        (seq_t)1,
        (seq_t)5,
        (seq_t)11,
        (seq_t)131,
        (seq_t)3421,
        (seq_t)56209,
        (seq_t)1049105,
        (seq_t)18561659,
        (seq_t)334918459
    };

    if (n < 9) {
        return init[(size_t)n];
    }

    /* Buffer holding the last seven computed terms:
       prev[0] = a(i-1), prev[1] = a(i-2), … , prev[6] = a(i-7).
       For the first iteration (i = 9) we initialise it with a(8),
       a(7), … , a(2).  This accommodates the recurrence
       which requires the previous seven terms. */
    seq_t prev[7];
    prev[0] = init[8];
    prev[1] = init[7];
    prev[2] = init[6];
    prev[3] = init[5];
    prev[4] = init[4];
    prev[5] = init[3];
    prev[6] = init[2];

    seq_t curr;
    for (idx_t i = 9; i <= n; ++i) {
        curr = (seq_t)13 * prev[0] + (seq_t)104 * prev[1]
             - (seq_t)260 * prev[2] - (seq_t)260 * prev[3]
             + (seq_t)104 * prev[4] + (seq_t)13 * prev[5]
             - prev[6];

        /* Rotate the window for the next iteration. */
        prev[6] = prev[5];
        prev[5] = prev[4];
        prev[4] = prev[3];
        prev[3] = prev[2];
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = curr;
    }

    return prev[0];
}
