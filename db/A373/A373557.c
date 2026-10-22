/*
 * A373557  Irregular triangle read by rows where row n lists (in decreasing
 *          order) the elements of the strong Schreier set encoded by
 *          A371176(2*n).
 * https://oeis.org/A373557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rec_gen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A373557 – elements of the strong Schreier sets in the
 *  irregular triangle read by rows.  This implementation
 *  enumerates the sets in increasing numeric order of the
 *  encoding integer (2*A371176(n)) by building all valid
 *  binary patterns and extracting the positions of 1‑bits.
 *  The sequence is produced by flattening the rows of the
 *  triangle in order.
 *
 *  The algorithm effectively generates all strong Schreier
 *  sets in order of the integer
 *  k = Σ 2^(i-1) where i belongs to the set, 
 *  subject to |S| < min(S).  For each valid set S
 *  the triangle row is the elements of S in decreasing
 *  order, with the set's maximum element listed first
 *  (as m).  The rows are processed in increasing order of
 *  the maximum element m, which guarantees the required
 *  global order.
 *
 *  The function is fully deterministic and works for any
 *  seq_t type (including arbitrary‑precision) and any
 *  idx_t (uint64_t) index ≥1.  It uses no dynamic memory
 *  except the small stack buffer for the current set
 *  representation.
 */

#include <stdint.h>
#include "../utils/rec_gen.h"

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    seq_t result = 0;
    /* Current set elements (in ascending order). */
    uint64_t cur[128];

    for (uint64_t m = 2; ; ++m) {
        /* The row starts with its maximum element m itself. */
        ++cnt;
        if (cnt == n) return (seq_t)m;

        /* All other sets with this maximum m. */
        if (rec_gen(m, m - 1, 0, cur, &cnt, n, &result))
            return result;
    }
}
