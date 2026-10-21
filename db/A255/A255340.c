/*
 * A255340  a(n) = H_n(4,3) where H_n is the n-th hyperoperator.
 * https://oeis.org/A255340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = H_n(4,3) where H_n is the n‑th hyperoperator
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Hyperoperator definition:
   H_0(x,y)=y+1,
   H_1(x,y)=x+y,
   H_2(x,y)=x*y,
   H_3(x,y)=x^y (big_pow),
   for n>=3 and y>0: H_n(x,y)=H_{n-1}(x, H_n(x, y-1)),
   base for y=0:
      n==1 -> x
      n==2 -> 0
      n>=3 -> 1
*/
#include "../utils/big_pow.h"

static seq_t hyper(idx_t n, seq_t x, seq_t y)
{
    /* Base case: H_0 */
    if (n == 0)
        return y + 1;

    /* Base for y==0 */
    if (y == 0) {
        if (n == 1)
            return x;
        if (n == 2)
            return (seq_t)0;
        return (seq_t)1;
    }

    /* Direct definitions for low levels */
    if (n == 1)
        return x + y;
    if (n == 2)
        return x * y;
    if (n == 3)
        return big_pow(x, y);

    /* Recurrence for higher levels */
    seq_t t = hyper(n, x, y - 1);
    return hyper(n - 1, x, t);
}

seq_t a(idx_t n)
{
    /* Compute H_n(4,3) as described */
    return hyper(n, (seq_t)4, (seq_t)3);
}
