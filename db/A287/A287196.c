/*
 * A287196  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 259", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n for even n; a(1) = 11; a(n) = (10^(n-1)-1)/9 for odd
 *            n >= 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0)               /* even n  */
        return pow10(n);

    if (n == 1)                     /* special case  */
        return 11;

    /* odd n >= 3 */
    seq_t t = pow10(n - 1);          /* 10^(n-1) */
    t -= 1;                          /* 10^(n-1) - 1 */
    t /= 9;                          /* (10^(n-1)-1)/9  */
    return t;
}
