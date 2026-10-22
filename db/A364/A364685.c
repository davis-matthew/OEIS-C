/*
 * A364685  The number of binary sequences of length n for which all patterns
 *          {0,1},{0,0},{1,0},{1,1} appear for the first time. In particular,
 *          three of the patterns will have appeared at least once before the
 *          (n-1)st digit in the sequence and the remaining pattern appears
 *          for the first and only time at posi...
 * https://oeis.org/A364685
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (n - 6 + F(n-1)) for n ≥ 5 where F(k) is the kth
 *            Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 5; for smaller n return 0. */
    if (n < 5) return 0;

    /* Compute the (n-1)th Fibonacci number. */
    seq_t f = fib(n - 1);

    /* n-6 may be negative (only for n = 5). Use signed arithmetic for the
       difference, then apply it to the Fibonacci value in the unsigned
       domain. */
    int64_t diff = (int64_t)n - 6;

    seq_t tmp;
    if (diff >= 0) {
        tmp = f + (seq_t)diff;
    } else {
        /* diff is negative and its magnitude is very small (at most 1
           for n ≥ 5).  Subtract the absolute value safely. */
        tmp = f - (seq_t)(-diff);
    }

    /* Multiply by 2 to obtain the final term. */
    return tmp * (seq_t)2;
}
