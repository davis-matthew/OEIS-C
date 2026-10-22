/*
 * A379279  G.f. A(x) satisfies A(x) = ( (1 + x*A(x)^2) * (1 + x*A(x)) )^2.
 * https://oeis.org/A379279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+2*k+2, k) * *
 *            binomial(2*n+2*k+2, n-k) / * (n+k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) -> seq_t */

seq_t a(idx_t n)
{
    /* Compute the sum directly. */
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t m = 2 * n + 2 * k + 2;          /* 2*n + 2*k + 2          */
        seq_t first  = binom(m, k);          /* binomial(2*n+2*k+2, k)  */
        seq_t second = binom(m, n - k);      /* binomial(2*n+2*k+2, n-k)*/
        seq_t denom  = (seq_t)(n + k + 1);   /* (n+k+1)                 */
        seq_t term   = (first * second) / denom;
        sum += term;
    }
    return sum;
}
