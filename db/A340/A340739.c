/*
 * A340739  The stopping time sequence for the 3x+1 function, restricted to
 *          its range and adjusted. Each term is the number of iterations of
 *          the function until it decreases.
 * https://oeis.org/A340739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min_{e>=1} f^e(x(n)) < x(n) where: x(n) = 6n+1 if n is
 *            odd (i.e., n % 2 == 1) 6n-1 if n is even f(x) = (3x+1) / 2^k
 *            with k the largest exponent such that 2^k divides (3x+1)
 * Range:     u8 n <= 4, u16 n <= 2244, u32 n <= 8194, u64 n <= 8194,
 *            u128 n <= 8194, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the stopping time for the 3x+1 function over the odd numbers in 6N±1. */

#include <stdbool.h>

/* Forward declaration of the step function */
static inline seq_t collatz_next(seq_t cur)
{
    seq_t t = cur * 3 + 1;          /* 3x+1 */
    /* remove all factors of 2 */
    while ((t & 1u) == 0u)
        t >>= 1;
    return t;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, return 0 */

    /* Compute the starting odd value x(n) */
    seq_t start;
    if ((n & 1u) != 0u)          /* n is odd: 6n+1 */
        start = (seq_t)n * 6 + 1;
    else                         /* n is even: 6n-1 */
        start = (seq_t)n * 6 - 1;

    seq_t cur = start;
    idx_t steps = 0;

    /* Iterate until we reach a value less than the start */
    while (true) {
        cur = collatz_next(cur);
        ++steps;
        if (cur < start)
            return (seq_t)steps;
    }
}
