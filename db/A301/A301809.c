/*
 * A301809  Group the natural numbers such that the first group is (1) then
 *          (2),(3),(4,5),(6,7,8),... with the n-th group containing F(n)
 *          sequential terms where F(n) is the n-th Fibonacci number
 *          (A000045(n)). Sequence gives the sum of terms in the n-th group.
 * https://oeis.org/A301809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = (F(n+2)+1)*F(n-1)/2
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* n == 1 (offset) */
        return (seq_t)1;

    seq_t fprev  = fib(n - 1);   /* F(n-1)   */
    seq_t fnext2 = fib(n + 2);   /* F(n+2)   */

    seq_t sum  = fnext2 + (seq_t)1;  /* F(n+2)+1 */
    seq_t prod = sum * fprev;        /* (F(n+2)+1)*F(n-1) */
    return prod / 2;                 /* divide by 2 */
}
