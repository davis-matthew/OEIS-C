/*
 * A348675  a(n) = Sum_{k=0..n-1} Omega(n^2-k^2).
 * https://oeis.org/A348675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} Omega(n^2 - k^2)
 * Range:     u8 n <= 53, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation evaluates the sum directly using the
   helper function bigomega(idx_t) from "../utils/bigomega.h".
   No memoisation or additional memory is required, and the
   code works for any build (primitive or arbitrary‑precision seq_t). */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* outside the defined offset; return 0 */
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k < n; ++k) {
        /* term = Omega(n - k) + Omega(n + k)  */
        seq_t term1 = bigomega(n - k);
        seq_t term2 = bigomega(n + k);
        sum += term1;
        sum += term2;
    }
    return sum;
}
