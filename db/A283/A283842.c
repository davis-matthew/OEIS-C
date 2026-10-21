/*
 * A283842  Expansion of x^3*(2-3*x)/((1-x)^2*(1-2*x)*(1-5*x+5*x^2)).
 * https://oeis.org/A283842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A283842
 *  g.f.: x^3*(2-3*x)/((1-x)^2*(1-2*x)*(1-5*x+5*x^2))
 *  Recurrence (n ≥ 5):
 *      a(n) = 9*a(n-1) - 30*a(n-2) + 47*a(n-3) - 35*a(n-4) + 10*a(n-5)
 *  Initial values:
 *      a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(4)=15
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1 || n == 2) return 0;
    if (n == 3) return 2;
    if (n == 4) return 15;

    /* last five terms: a(n-5) ... a(n-1) */
    seq_t a0 = 0,  /* a(0)  */
          a1 = 0,  /* a(1)  */
          a2 = 0,  /* a(2)  */
          a3 = 2,  /* a(3)  */
          a4 = 15; /* a(4)  */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i) {
        /* compute a(i) = 9*a4 - 30*a3 + 47*a2 - 35*a1 + 10*a0
           To avoid underflow on unsigned seq_t, add positives first,
           then subtract the sum of negative terms. */
        cur = 9 * a4 + 47 * a2 + 10 * a0;
        cur -= (30 * a3 + 35 * a1);

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }

    return a4;
}
