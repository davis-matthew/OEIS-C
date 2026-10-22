/*
 * A309408  Start with X = F(n) = A000045(n). Repeatedly replace X with X -
 *          ceiling(X/n); a(n) is the number of steps to reach 0.
 * https://oeis.org/A309408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of iterations of X_{k+1} = floor((n-1) * X_k / n)
 *            starting from X_0 = F(n) (Fibonacci) until X_k becomes 0
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 40, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* F(0)=0, already zero */

    seq_t X = fib(n);                  /* starting value F(n) */
    seq_t steps = 0;
    seq_t nn = (seq_t)n;               /* convert index to seq_t */
    seq_t nm1 = nn - 1;                /* n-1 as seq_t */

    while (X > 0) {
        /* X = floor((n-1) * X / n) */
        X = (X * nm1) / nn;
        steps++;
    }
    return steps;
}
