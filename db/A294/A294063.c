/*
 * A294063  Sum of the differences of the larger and smaller parts in the
 *          partitions of 2n into two parts with the larger part squarefree.
 * https://oeis.org/A294063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Σ_{i=1}^{n} (n-i) * μ(2n-i)^2
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the helper function is_squarefree from ../utils/is_squarefree.h,
 * which returns true iff its argument is squarefree.  Since μ(m)^2 is 1 for
 * squarefree m and 0 otherwise, the summand is simply (n-i) whenever
 * 2n-i is squarefree.  The sum is multiplied by 2 at the end.  The loop
 * runs in O(n) time and uses only integer arithmetic. */

#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;       /* running total */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t m = 2 * n - i;             /* 2n - i, fits in idx_t for all intended n */
        if (is_squarefree((seq_t)m)) {
            sum += (seq_t)(n - i);      /* add (n-i) */
        }
    }
    return sum * (seq_t)2;   /* multiply final sum by 2 */
}
