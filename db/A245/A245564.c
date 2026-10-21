/*
 * A245564  a(n) = Product_{i in row n of A245562} Fibonacci(i+2).
 * https://oeis.org/A245564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of Fibonacci(runlength+2) over all runs of 1's in
 *            the binary expansion of n
 * Range:     u8 n <= 3070, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the Run‑Length Transform of Fibonacci(n+2).
   For each run of consecutive 1-bits of length L in the binary
   representation of n, multiply by Fibonacci(L+2).  If n = 0,
   the product is defined to be 1 (empty product). */

#include <stdbool.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                /* a(0) = 1 */

    seq_t result = (seq_t)1;
    idx_t  temp   = n;

    while (temp != 0)
    {
        /* Skip zero bits */
        while ((temp & (idx_t)1) == 0)
        {
            temp >>= 1;
            if (temp == 0)
                return result;          /* no more runs of 1's */
        }

        /* Count a run of consecutive 1's */
        unsigned runlen = 0;
        while ((temp & (idx_t)1) == 1)
        {
            runlen++;
            temp >>= 1;
        }

        /* Multiply by Fibonacci(runlen+2) */
        result *= fib((idx_t)(runlen + 2));
    }

    return result;
}
