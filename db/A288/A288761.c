/*
 * A288761  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 501", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=100, a(3)=111, a(4)=11000, a(5)=11111. For
 *            n>5: if n is even: a(n)=11*10^(n-1) if n is odd :
 *            a(n)=(10^n-10)/90 The pow10 utility (@../utils/pow10.h) is used
 *            to compute 10^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Handle the small base cases directly */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 100;
    if (n == 3) return 111;
    if (n == 4) return 11000;
    if (n == 5) return 11111;

    /* For n > 5 use the closed formulas */
    if ((n & 1ULL) == 0)            /* even n */
    {
        /* a(n) = 11 * 10^(n-1) */
        return 11 * pow10(n - 1);
    }
    else                            /* odd n */
    {
        /* a(n) = (10^n - 10) / 90 */
        seq_t tmp = pow10(n);
        tmp -= 10;
        return tmp / 90;
    }
}
