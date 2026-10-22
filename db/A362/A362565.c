/*
 * A362565  The number of linear extensions of n fork-join DAGs of width 4.
 * https://oeis.org/A362565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n)! / 30^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t result = (seq_t)1;

    for (idx_t j = 1; j <= n; ++j)
    {
        /* Compute (6*j)(6*j-1)...(6*j-5) / 30 */
        seq_t prod = (seq_t)1;
        idx_t top = 6 * j;
        for (idx_t k = 0; k < 6; ++k)
        {
            prod *= (seq_t)(top - k);
        }
        prod /= (seq_t)30;              /* Exact division */

        result *= prod;                 /* result now holds a(j) */
    }

    return result;
}
