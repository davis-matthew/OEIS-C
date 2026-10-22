/*
 * A390246  a(n) is the least integer k such that there exist n distinct
 *          integers b_1, ..., b_n with n < b_i < n+k and b_i is divisible by
 *          i for 1 <= i <= n.
 * https://oeis.org/A390246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) is computed by binary searching minimal k such that a
 *            bipartite matching exists between the indices 1..n (left side)
 *            and the integers n+1..n+k-1 (right side), where an edge (i, v)
 *            exists iff v is divisible by i
 * Range:     u8 n <= 155, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_uv, has_matching
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Required utilities are not used.  All operations are performed with
    idx_t for indices and seq_t only for the result.               */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/dfs_uv.h"
#include "../utils/has_matching.h"

/* Forward declaration of helper */
static bool has_matching(idx_t n, idx_t k);

/* Main entry function required by the judge. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not defined, but safe */
    idx_t low = 2;                  /* smallest possible k */
    idx_t high = 3 * n;             /* safe upper bound (k ≤ 3n) */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (has_matching(n, mid))
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
