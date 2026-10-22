/*
 * A343010  Integers k for which there exist three consecutive Fibonacci
 *          numbers a, b, and c such that a*b*c = k*(a+b+c).
 * https://oeis.org/A343010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0 For n >= 2: let k = n / 2 // integer division if n is
 *            even: a(n) = F(3k-1) * F(3k) / 2 else (n is odd): a(n) = F(3k+1)
 *            * F(3k) / 2 where F(m) is the m‑th Fibonacci number (F(0)=0,
 *            F(1)=1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"   /* provides seq_t fib(idx_t) */

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    idx_t k = n / 2;                 /* k >= 1 for n >= 2 */
    seq_t fn = fib(3 * k);           /* F(3k) */

    if ((n & 1) == 0) {              /* n even -> use F(3k-1) */
        seq_t f1 = fib(3 * k - 1);   /* F(3k-1) */
        return (f1 * fn) / 2;
    } else {                         /* n odd -> use F(3k+1) */
        seq_t f2 = fib(3 * k + 1);   /* F(3k+1) */
        return (f2 * fn) / 2;
    }
}
