/*
 * A368580  a(n) = Sum_{d|n and d^2 <= n} (1 + [d^2 < n]) * (2*d - 1), where
 *          [.] denote the Iverson brackets.
 * https://oeis.org/A368580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n and d^2 <= n} (1 + [d^2 < n]) * (2*d - 1)
 * Range:     u8 n <= 335, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: enumerate divisors d <= sqrt(n); for each divisor add
   2*(2*d-1) if d^2 < n (i.e., d not the sqrt of a perfect square),
   otherwise add (2*d-1). Time complexity O(sqrt(n)). */

#include <stdint.h>    /* for uint64_t, if used */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d)   /* iterate d <= floor(sqrt(n)) */
    {
        if (n % d == 0)
        {
            seq_t d_seq = (seq_t)d;
            seq_t term = 2 * d_seq - 1;  /* 2*d - 1 */
            if (d * d < n)              /* d is not the square root */
                sum += 2 * term;        /* weight 2 */
            else
                sum += term;            /* weight 1 */
        }
    }
    return sum;
}
