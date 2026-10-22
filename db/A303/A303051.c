/*
 * A303051  Number of partitions of n into two distinct parts (p,q) such that
 *          p, q and p+q are all squarefree.
 * https://oeis.org/A303051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} mu(n)^2 * mu(i)^2 * mu(n-i)^2
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (!is_squarefree((seq_t)n))
        return 0;

    idx_t limit = (n - 1) / 2;
    seq_t res = 0;
    for (idx_t i = 1; i <= limit; ++i)
    {
        if (is_squarefree((seq_t)i) && is_squarefree((seq_t)(n - i)))
            ++res;
    }
    return res;
}
