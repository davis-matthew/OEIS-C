/*
 * A257133  a(n) = cpg(3, n) * cpg(4, n) * ... * cpg(n, n) where cpg(m, n) is
 *          the n-th m-th-order centered polygonal number.
 * https://oeis.org/A257133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{m=3}^{n} ( m*n*(n-1)/2 + 1 )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Centered m‑gonal number: cpg(m,n) = m*n*(n-1)/2 + 1   */

#include <stdbool.h>
#include <stdint.h>

/* Computes the OEIS sequence A257133. */
seq_t a(idx_t n)
{
    /* For n = 0,1,2 the product is empty → 1. */
    if (n < 3)
        return (seq_t)1;

    /* Cast index i to seq_t once to avoid repeated conversions. */
    const seq_t nn  = (seq_t)n;
    const seq_t nn1 = (seq_t)(n - 1);
    seq_t result = (seq_t)1;

    /* Multiply the centered polygonal numbers for m = 3 … n. */
    for (idx_t m = 3; m <= n; ++m)
    {
        const seq_t mm   = (seq_t)m;
        /* term = m*n*(n-1)/2 + 1 */
        const seq_t term = mm * nn * nn1 / (seq_t)2 + (seq_t)1;
        result *= term;
    }
    return result;
}
