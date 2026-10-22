/*
 * A337727  a(n) = (4*n)! * Sum_{k=0..n} 1 / (4*k)!.
 * https://oeis.org/A337727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n)! * Σ_{k=0..n} 1/(4*k)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n = 0 is handled automatically by the following code.    */
    /* Compute (4*n)! as the initial term.                       */
    idx_t m = 4 * n;                /* m = 4n, the largest factorial needed */
    seq_t term = (seq_t)1;          /* term = (4n)! */
    for (idx_t i = 1; i <= m; ++i)
        term *= (seq_t)i;           /* multiply by i */

    seq_t sum = term;              /* sum starts with k = 0 term */

    /* Build the remaining terms of the sum by successive division. */
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t tk = 4 * k;           /* 4k */
        seq_t prod = (seq_t)tk;     /* tk * (tk-1) * (tk-2) * (tk-3) */
        prod *= (seq_t)(tk - 1);
        prod *= (seq_t)(tk - 2);
        prod *= (seq_t)(tk - 3);

        term /= prod;              /* t_k = t_{k-1} / prod */
        sum += term;               /* accumulate the sum */
    }

    return sum;
}
