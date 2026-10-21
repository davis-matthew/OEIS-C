/*
 * A262870  Sum of the squarefree numbers appearing among the larger parts of
 *          the partitions of n into two parts.
 * https://oeis.org/A262870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋} (n-i) * μ(n-i)^2 where μ is the Möbius
 *            function; μ(d)^2 = 1 iff d is square‑free
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = n / 2;          /* floor(n/2) */
    for (idx_t i = 1; i <= limit; ++i) {
        idx_t m = n - i;          /* larger part of partition (m, i) */
        if (is_squarefree((seq_t)m))
            sum += (seq_t)m;
    }
    return sum;
}
