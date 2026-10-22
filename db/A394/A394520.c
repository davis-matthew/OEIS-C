/*
 * A394520  E.g.f. A(x) satisfies A(x) = 1/(1 - 3*x*A(x)^2)^(1/3).
 * https://oeis.org/A394520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=1}^{n-1} (5*n+1 - 3*k), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for completeness; not strictly needed */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return 1;

    /* Compute the product \prod_{k=1}^{n-1} (5*n+1 - 3*k) */
    seq_t prod  = 1;
    seq_t base  = (seq_t)(5 * (seq_t)n + 1);

    for (idx_t k = 1; k < n; ++k) {
        /* term = base - 3*k  */
        seq_t term = base - (seq_t)3 * k;
        prod = prod * term;
    }

    return prod;
}
