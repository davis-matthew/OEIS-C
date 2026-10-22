/*
 * A390266  a(n) = Sum_{k=0..n} binomial(2*n+k+1,n-k).
 * https://oeis.org/A390266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+k+1, n-k) (computed via the
 *            equivalent convolution with Fibonacci numbers)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /*  We use the identity
          a(n) = Σ_{k=0..n} C(2n+1, n-k)·F_{k+1}
       where F_m is the m-th Fibonacci number (F_1=1,F_2=1).
       For each n we generate the binomial coefficients C(2n+1,m)
       for m=n,n-1,…,0 by the recurrence
           C(N,m-1) = C(N,m) * m / (N-m+1).
       Fibonacci numbers are produced on the fly.
    */
    seq_t result = 0;

    /* Base cases: n==0 => result = 1, handled by loop below */
    idx_t N = (idx_t)(2 * n + 1);      /* N = 2n+1 */
    idx_t m = n;                       /* current m in C(N,m)       */
    seq_t binom_val = binomial(N, m);   /* C(N,n) */
    seq_t fib_prev = 0;                 /* F_0 = 0 */
    seq_t fib_curr = 1;                 /* F_1 = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        /* Add contribution for current k */
        result += binom_val * fib_curr;

        if (k == n) break;            /* last term processed */

        /* Update binomial coefficient to the next m-1 */
        seq_t num = m;                            /* numerator factor m   */
        seq_t den = (seq_t)(N - m + 1);           /* denominator N-m+1   */
        binom_val = (binom_val * num) / den;

        --m; /* move to next m (decreasing) */

        /* Advance Fibonacci numbers (next F_{k+2}) */
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }

    return result;
}
