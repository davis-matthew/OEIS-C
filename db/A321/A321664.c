/*
 * A321664  A sequence consisting of three disjoint copies of the Fibonacci
 *          sequence, one shifted, with the property that for any four
 *          consecutive terms the maximum term is the sum of the two minimum
 *          terms.
 * https://oeis.org/A321664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(3*n) = F(n+2) - 1 Formula: a(3*n+1) = F(n+2) Formula: a(3*n+2)
 *            = F(n+1) where F(k) is the k-th Fibonacci number with F(0)=0,
 *            F(1)=1. (The recurrence a(n)=2*a(n-3)-a(n-9) is equivalent.)
 *            This implementation uses the provided fib() utility to obtain
 *            Fibonacci numbers of index up to n+2. The funct...
 * Range:     u8 n <= 35, u16 n <= 68, u32 n <= 137, u64 n <= 275,
 *            u128 n <= 554, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    idx_t k = n / 3;
    int r = n % 3;
    seq_t result;

    switch (r) {
    case 0:
        result = fib(k + 2);
        result--;                  /* F(k+2) - 1 */
        break;
    case 1:
        result = fib(k + 2);
        break;
    case 2:
        result = fib(k + 1);
        break;
    default:
        /* unreachable */
        result = 0;
        break;
    }
    return result;
}
