/*
 * A343008  a(n) = F(n+5) * F(n+2) - 12 * (-1)^n where F(n) = A000045(n) are
 *          the Fibonacci numbers.
 * https://oeis.org/A343008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n+5) * F(n+2) - 12 * (-1)^n
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci numbers F(n+5) and F(n+2) */
    seq_t f1 = fib(n + 5);  /* F(n+5) */
    seq_t f2 = fib(n + 2);  /* F(n+2) */

    /* Product F(n+5)*F(n+2) */
    seq_t prod = f1 * f2;

    /* Subtract or add 12 depending on the parity of n */
    if ((n & 1) == 0)          /* n even -> (-1)^n = +1, subtract 12 */
        return prod - (seq_t)12;
    else                       /* n odd  -> (-1)^n = -1, add 12 */
        return prod + (seq_t)12;
}
