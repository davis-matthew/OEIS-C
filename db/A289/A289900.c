/*
 * A289900  Number of maximal matchings in the n-triangular honeycomb rook
 *          graph.
 * https://oeis.org/A289900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} A001147(ceil(k/2))
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A001147 here is the odd double‑factorial (1,3,15,105,945,…). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* safety for offsets ≤0 */
    idx_t m = (n + 1) / 2;                 /* ceil(n/2) */
    seq_t result = (seq_t)1;
    seq_t w = (seq_t)1;                    /* product of odd numbers = A001147(i) */

    for (idx_t i = 1; i <= m; ++i)
    {
        seq_t odd = (seq_t)((2 * i) - 1);
        w *= odd;                          /* compute A001147(i) */
        result *= w;                       /* first power */
        /* second power if i < m or i == m and n is even */
        if (i < m || (i == m && (n & (idx_t)1) == 0))
        {
            result *= w;
        }
    }
    return result;
}
