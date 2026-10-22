/*
 * A344370  Dirichlet g.f.: Product_{k>=2} (1 + k^(1-s)).
 * https://oeis.org/A344370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * number of ways to express n as a product of distinct
 *            integers ≥ 2 (including n itself)
 * Range:     u8 n <= 47, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   count_distinct_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_parts.h"

seq_t a(idx_t n)
{
    if (n == 1)                      /* a(1) = 1 */
        return (seq_t)1;
    seq_t cnt = count_distinct_parts(n, 1);
    return (seq_t)n * cnt;
}
