/*
 * A291413  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - 3 S + S^2 + S^3.
 * https://oeis.org/A291413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 3, a(1) = 11, a(2) = 36, a(3) = 117, a(4) = 375, a(5) =
 *            1197, a(6) = 3810 a(n) = 3*a(n-1) + 2*a(n-2) - 3*a(n-3) -
 *            4*a(n-4) - 3*a(n-5) - a(n-6) for n ≥ 7
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial seven terms */
    static const seq_t init[7] = {
        (seq_t)3, (seq_t)11, (seq_t)36, (seq_t)117,
        (seq_t)375, (seq_t)1197, (seq_t)3810
    };

    if (n < 7)
        return init[n];

    /* variables holding a(n-6) .. a(n-1) */
    seq_t a0 = init[0];     /* a(n-6) */
    seq_t a1 = init[1];     /* a(n-5) */
    seq_t a2 = init[2];     /* a(n-4) */
    seq_t a3 = init[3];     /* a(n-3) */
    seq_t a4 = init[4];     /* a(n-2) */
    seq_t a5 = init[5];     /* a(n-1) */
    seq_t a6 = init[6];     /* a(n)   */

    seq_t an;
    for (idx_t i = 7; i <= n; ++i)
    {
        /* compute next term using recurrence:
           an = 3*a6 + 2*a5 - 3*a4 - 4*a3 - 3*a2 - a1
           The sum of the positive terms is always ≥ the sum of the negatives,
           so subtraction is safe when a is unsigned. */
        an = (3 * a6) + (2 * a5);

        an = an - (3 * a4);
        an = an - (4 * a3);
        an = an - (3 * a2);
        an = an - a1;

        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = an;
    }

    return a6;
}
