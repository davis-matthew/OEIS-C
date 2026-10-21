/*
 * A280252  Sum of the parts in the partitions of 2n into two squarefree
 *          parts.
 * https://oeis.org/A280252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*A280226(n) (A280226(n) = number of unordered pairs
 *            (a,b) with a+b=2n, a,b squarefree). This function returns the
 *            sum of the parts in all such partitions; each partition
 *            contributes 2n, so the sum equals 2n times the count of
 *            partitions
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* 2*n in seq_t */
    seq_t two_n = (seq_t)2 * (seq_t)n;

    seq_t count = 0;
    /* iterate over a from 1 to n (inclusive) so that a <= b = 2n-a */
    for (idx_t a = 1; a <= n; ++a) {
        seq_t part_a = (seq_t)a;
        seq_t part_b = two_n - part_a;
        if (is_squarefree(part_a) && is_squarefree(part_b))
            ++count;
    }
    return two_n * count;
}
