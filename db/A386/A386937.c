/*
 * A386937  a(n) = Sum_{k=0..n} binomial(3*n+1,k) * binomial(2*n-k-1,n-k).
 * https://oeis.org/A386937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n+1,k) * binomial(2*n-k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    seq_t total = (seq_t)0;
    /* Compute each term separately using binomials.  For the
       64‑bit build this works for all n for which the final
       result fits in uint64_t (n <= 19).  For arbitrary‑precision
       builds the intermediate products are handled by the
       big‑integer implementation of binomial. */
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t c1 = binomial((idx_t)(3 * n + 1), k);
        seq_t c2 = binomial((idx_t)(2 * n - k - 1), n - k);
        seq_t term = c1 * c2;
        total += term;
    }

    return total;
}
