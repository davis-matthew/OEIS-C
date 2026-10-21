/*
 * A248928  Interleave (2*n+2)^2 with (2*n+3)^2, both listed n+1 times.
 * https://oeis.org/A248928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A027434(n+1)^2
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   group_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/group_k.h"

/* Return the n-th term of OEIS sequence A248928. */
seq_t
a(idx_t n)
{
    /* Determine the group k for this index. */
    idx_t k = group_k(n);

    /* Starting index of this group. */
    idx_t start = (k - 1) * k;

    /* Position within the group. */
    idx_t offset = n - start;

    /* Base value for the square root. */
    seq_t val = (seq_t)((offset < k) ? 2 * k : 2 * k + 1);

    /* Return the square. */
    return val * val;
}
