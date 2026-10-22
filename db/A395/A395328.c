/*
 * A395328  Bipartite dimension of the n-barbell graph.
 * https://oeis.org/A395328
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + ceil(log_2(n - 1)) + ceil(log_2(n))
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* For n < 3 the behaviour is irrelevant to the OEIS data set */
    if (n < 3)
        return 0;

    /* Compute the two ceil(log2) terms.  For integers x >= 2,
       ceil(log2(x)) = floor_log2_uint64(x - 1) + 1.  */
    seq_t c1 = (seq_t)floor_log2_uint64((n - 1) - 1) + 1;
    seq_t c2 = (seq_t)floor_log2_uint64((n) - 1) + 1;

    return (seq_t)1 + c1 + c2;
}
