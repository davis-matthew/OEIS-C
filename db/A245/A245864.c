/*
 * A245864  Number of length n+2 0..2 arrays with some pair in every
 *          consecutive three terms totalling exactly 2.
 * https://oeis.org/A245864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+a(n-2)-a(n-4)-a(n-5) with base terms a(1)=19,
 *            a(2)=45, a(3)=103, a(4)=239, a(5)=553, a(6)=1281 The recurrence
 *            holds for all n ≥ 7
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 50, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, return 0 for safety */

    /* Return base cases directly */
    if (n <= 6) {
        static const seq_t base[6] = {19, 45, 103, 239, 553, 1281};
        return base[(idx_t)(n - 1)];
    }

    /* Sliding window of the last six terms:
         a1 = a(n-6), a2 = a(n-5), a3 = a(n-4),
         a4 = a(n-3), a5 = a(n-2), a6 = a(n-1)  */
    seq_t a1 = 19,    /* a(1)  */
          a2 = 45,    /* a(2)  */
          a3 = 103,   /* a(3)  */
          a4 = 239,   /* a(4)  */
          a5 = 553,   /* a(5)  */
          a6 = 1281;  /* a(6)  */
    seq_t ai = 0;

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) = 2*a(i-1) + a(i-2) - a(i-4) - a(i-5) */
        ai = (seq_t)2 * a6 + a5 - a3 - a2;

        /* Shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = ai;
    }

    return ai;
}
