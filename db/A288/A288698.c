/*
 * A288698  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 494", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A288698:
   a(0) = 1
   a(1) = 10
   a(2) = 0
   for n >= 3:
        a(n) = 10^n      if n is odd
        a(n) = 10^(n-1)  if n is even
   (The exponent is the largest odd integer ≤ n, except n = 2.)
   The function uses the utility pow10 which returns 10^e in a `seq_t`. */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* 1  */
        return pow10(0);
    if (n == 1)          /* 10 */
        return pow10(1);
    if (n == 2)          /* 0  */
        return (seq_t)0;

    /* For n >= 3, choose the largest odd integer ≤ n */
    if (n & (idx_t)1)    /* odd n */
        return pow10(n);

    /* even n >= 4 */
    return pow10(n - 1);
}
