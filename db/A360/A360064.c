/*
 * A360064  Number of 3-dimensional tilings of a 2 X 2 X n box using 1 X 1 X 1
 *          cubes and trominos (L-shaped connection of 3 cubes).
 * https://oeis.org/A360064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence 1 from the OEIS entry for A360064.
   Initial values:
     a(0) = 1
     a(1) = 5
     a(2) = 89
     ...
   Recurrence (for n ≥ 1):
     a(n) = 5*a(n-1) + 2*b(n-1) + c(n-1) + d(n-1) + e(n-1)
            + 8*a(n-2) + 4*b(n-2) + c(n-2) + 2*d(n-2)
     b(n) = 8*a(n-1) + 4*b(n-1) + 2*c(n-1)
     c(n) = 20*a(n-1) + 6*b(n-1) + 4*c(n-1) + 4*d(n-1) + 2*e(n-1)
     d(n) = 4*a(n-1)
     e(n) = 16*a(n-1) + 4*b(n-1)
   For n ≤ 0 all a,b,c,d,e are 0 except a(0)=1.
   The function is iterative, uses only 5 previous states and
   runs in O(n) time. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1; /* a(0) = 1 */

    /* State for n-2 */
    seq_t aPrevPrev = 0, bPrevPrev = 0, cPrevPrev = 0, dPrevPrev = 0, ePrevPrev = 0;
    /* State for n-1 (initially n-1 = 0) */
    seq_t aPrev = 1, bPrev = 0, cPrev = 0, dPrev = 0, ePrev = 0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t new_a = 5*aPrev + 2*bPrev + cPrev + dPrev + ePrev
                    + 8*aPrevPrev + 4*bPrevPrev + cPrevPrev + 2*dPrevPrev;
        seq_t new_b = 8*aPrev + 4*bPrev + 2*cPrev;
        seq_t new_c = 20*aPrev + 6*bPrev + 4*cPrev + 4*dPrev + 2*ePrev;
        seq_t new_d = 4*aPrev;
        seq_t new_e = 16*aPrev + 4*bPrev;

        /* shift states */
        aPrevPrev = aPrev; bPrevPrev = bPrev; cPrevPrev = cPrev;
        dPrevPrev = dPrev; ePrevPrev = ePrev;
        aPrev = new_a; bPrev = new_b; cPrev = new_c;
        dPrev = new_d; ePrev = new_e;
    }
    return aPrev;
}
