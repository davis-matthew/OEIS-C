/*
 * A309913  Distance from n to closest squarefree number that is different
 *          from n.
 * https://oeis.org/A309913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute the distance from n to the nearest squarefree number that is different from n.
 * This is equivalent to:
 *   a(n) = min_{m≠n, m squarefree} |n - m|
 *
 * Algorithm: Search outward from n. For each distance k = 1, 2, 3, ... test
 *     n - k (if non‑negative) and n + k.  The first squarefree number found
 *     gives the desired distance.
 *
 * Complexity: O(gap) per query where "gap" is the distance to the nearest
 * squarefree number.  For n ≤ 10000 this gap is at most a few tens.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Search outward for the nearest squarefree number, excluding n itself */
    for (idx_t k = 1; ; ++k)
    {
        if (n >= k)
        {
            /* candidate = n - k (always non‑negative) */
            if (is_squarefree((seq_t)(n - k)))
                return (seq_t)k;
        }
        /* candidate = n + k */
        if (is_squarefree((seq_t)(n + k)))
            return (seq_t)k;
        /* loop continues until a squarefree number is found */
    }
}
