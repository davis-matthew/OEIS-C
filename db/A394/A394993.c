/*
 * A394993  a(n) = n! * Sum_{k=0..n} (-1)^(k+1)*F(k)/k!, where F(n) is the
 *          n-th Fibonacci number, A000045(n).
 * https://oeis.org/A394993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Σ_{k=0..n} (-1)^(k+1) * F(k) / k!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = (-1)^(n+1) * F(n) + n * a(n-1)   (a(0)=0) */

#include <stdint.h>   /* for uint64_t used by idx_t, if needed */

seq_t a(idx_t n)
{
    /* Fast iterative computation using the recurrence
       a(i) = (-1)^(i+1)*F(i) + i*a(i-1)          (i ≥ 1),
       with a(0)=0.  We generate Fibonacci numbers
       F(1)=1, F(2)=1, ... on the fly.            */
    if (n == 0)
        return 0;

    seq_t result = 0;               /* a(0)   */
    seq_t fib_prev = 0;             /* F(0)   */
    seq_t fib_curr = 1;             /* F(1)   */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* sign = (-1)^(i+1)  -> +1 for odd i, -1 for even i */
        if (i & 1)
            result = (seq_t)i * result + fib_curr;   /* +F(i) */
        else
            result = (seq_t)i * result - fib_curr;   /* -F(i) */

        /* next Fibonacci number for next iteration */
        seq_t next = fib_prev + fib_curr;
        fib_prev   = fib_curr;
        fib_curr   = next;
    }
    return result;
}
