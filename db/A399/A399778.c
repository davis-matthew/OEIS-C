/*
 * A399778  Diagonal product divided by the central-cross sum in the (2*n + 1)
 *          X (2*n + 1) square array formed by the integers 1, 2, ..., (2*n +
 *          1)^2 in order.
 * https://oeis.org/A399778
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = (Product_{k = 1..n} (c^2 - 4*n^2*k^2)*(c^2 - 4*(n +
 *            1)^2*k^2))/(4*n + 1) where c = 2*n^2 + 2*n + 1. The function
 *            uses the helper seq_gcd(seq_t a, seq_t b) for partial
 *            cancellation with the (4*n+1) divisor to keep intermediate sizes
 *            smaller
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 2; for n < 2 the value is not defined. */
    if (n < 2)
        return 0;

    seq_t nn   = (seq_t)n;
    seq_t c    = 2 * nn * nn + 2 * nn + 1;     /* c = 2*n^2 + 2*n + 1 */
    seq_t denom = 4 * nn + 1;                  /* divisor */

    seq_t prod = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;

        /* terms of the product */
        seq_t term1 = c * c - 4 * nn * nn * kk * kk;
        seq_t term2 = c * c - 4 * (nn + 1) * (nn + 1) * kk * kk;

        /* cancel with remaining part of the divisor */
        seq_t g = seq_gcd(term1, denom);
        if (g > 1) {
            term1 /= g;
            denom /= g;
        }
        prod *= term1;

        g = seq_gcd(term2, denom);
        if (g > 1) {
            term2 /= g;
            denom /= g;
        }
        prod *= term2;
    }

    /* each valid term makes sure the final denominator is 1; if not,
       perform the remaining division (it must divide exactly). */
    if (denom > 1)
        prod /= denom;

    return prod;
}
