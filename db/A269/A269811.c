/*
 * A269811  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 33", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = -2 + (-1)^n/3 - (5*2^(n+1))/3 - 5*3^(n-2) + 13*4^(n-1)
 *            (for n > 1, a(0)=1, a(1)=4)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* compute numerator: 39*4^(n-1) - 15*3^(n-2) - 5*2^(n+1) - 6 + (-1)^n */
    seq_t num = (seq_t)39 * pow4(n - 1);              /* 39*4^(n-1)              */
    num -= (seq_t)15 * pow3(n - 2);                  /* -15*3^(n-2)              */
    num -= (seq_t)5  * pow2(n + 1);                  /* -5*2^(n+1)               */
    num -= (seq_t)6;                                 /* -6                        */
    if ((n & (idx_t)1) == 0)                         /* (-1)^n = +1 if n even   */
        num += (seq_t)1;
    else                                               /* (-1)^n = -1 if n odd    */
        num -= (seq_t)1;

    /* divide by 3 to get the final value */
    return num / (seq_t)3;
}
