/*
 * A285650  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 73", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=4, a(3)=3; For n>3: if n is even, a(n)=0;
 *            if n is odd, a(n)=2^(n+1)-1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Base cases for n = 0…3 */
    if (n == 0 || n == 1)                 return (seq_t)1;
    if (n == 2)                           return (seq_t)4;
    if (n == 3)                           return (seq_t)3;

    /* For n > 3 */
    if ((n & 1) == 0)                     /* even */
        return (seq_t)0;
    else                                  /* odd */
    {
        seq_t val = pow2((idx_t)n + 1);   /* 2^(n+1) */
        val -= (seq_t)1;                  /* subtract 1 */
        return val;
    }
}
