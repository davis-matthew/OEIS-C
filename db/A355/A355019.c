/*
 * A355019  Partial sums of L(1) - F(1) + L(2) - F(2) + L(3) - F(3) + ...,
 *          where L = A000032 and F = A000045.
 * https://oeis.org/A355019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -2 + 2*F((n+3)/2) for odd n * a(n) = -2 + F((n+8)/2) for
 *            even n * where F(k) is the k-th Fibonacci number (F(0)=0,
 *            F(1)=1)
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 87, u64 n <= 179,
 *            u128 n <= 365, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n & 1ULL) {               /* n is odd */
        idx_t m = (n + 3ULL) / 2ULL;
        seq_t f = fib(m);
        return f + f - 2;
    } else {                      /* n is even */
        idx_t m = (n + 8ULL) / 2ULL;
        seq_t f = fib(m);
        return f - 2;
    }
}
