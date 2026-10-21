/*
 * A294210  Sum of the larger parts in the partitions of n into two squarefree
 *          parts or two nonsquarefree parts.
 * https://oeis.org/A294210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋} (n-i) * [is_squarefree(i) ==
 *            is_squarefree(n-i)]
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* offset starts at n = 1 */

    seq_t sum = 0;
    idx_t half = n / 2;

    for (idx_t i = 1; i <= half; ++i) {
        bool sf_i = is_squarefree((seq_t)i);
        idx_t j = n - i;
        bool sf_j = is_squarefree((seq_t)j);

        if (sf_i == sf_j)
            sum += (seq_t)j; /* j = n - i is the larger part */
    }

    return sum;
}
