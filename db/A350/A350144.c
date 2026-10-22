/*
 * A350144  a(n) = Sum_{k=1..n} floor(n/(2*k-1))^3.
 * https://oeis.org/A350144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/(2*k-1))^3 Computed by reducing the
 *            loop to the range where the divisors are ≤ n
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1598, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* Sequence is defined only for n ≥ 1, but keep defensive. */

    idx_t limit = (n + 1) / 2;          /* for 2*k-1 > n the quotient is zero */
    seq_t sum = 0;

    for (idx_t k = 1; k <= limit; ++k)
    {
        idx_t odd   = 2 * k - 1;               /* 2*k-1 is guaranteed ≤ n */
        idx_t q     = n / odd;                 /* floor division as idx_t */
        seq_t t     = (seq_t)q;                /* expose quotient as seq_t          */
        seq_t t2    = t * t;                   /* t^2                               */
        seq_t t3    = t2 * t;                  /* t^3                               */
        sum += t3;                            /* accumulate sum                    */
    }

    return sum;
}
