/*
 * A262991  Number of squarefree numbers among the parts of the partitions of
 *          n into two parts.
 * https://oeis.org/A262991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=1..floor(n/2)} [is_squarefree(i) +
 *            is_squarefree(n-i)]
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t half = n >> 1;                     /* floor(n/2) */
    seq_t res = 0;

    for (idx_t i = 1; i <= half; ++i)
    {
        seq_t part1 = (seq_t)i;
        seq_t part2 = (seq_t)(n - i);

        if (is_squarefree(part1)) ++res;
        if (is_squarefree(part2)) ++res;
    }

    return res;
}
