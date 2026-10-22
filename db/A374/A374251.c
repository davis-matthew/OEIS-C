/*
 * A374251  Irregular triangle read by rows where row n is the run-compression
 *          of the n-th composition in standard order.
 * https://oeis.org/A374251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   dfs_comp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A374251  :  irregular triangle read by rows
 *
 *  The sequence a(n) is the stream of run‑compressed parts of
 *  compositions listed in standard order (A066099).  The algorithm
 *  enumerates compositions by increasing sum and, within each sum,
 *  by reverse‐lexicographic order.  For every composition it
 *  computes its run‑compression and emits the parts one by one.
 *  The routine returns the n‑th term of that stream.
 */
#include <stddef.h>
#include "../utils/dfs_comp.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* OEIS offset is 1, but we guard against 0 */
        return 0;

    idx_t count = 0;
    seq_t result = 0;
    int parts[64];                      /* enough for required sums  */

    for (idx_t sum = 1; ; ++sum) {
        if (dfs_comp(sum, parts, 0, &count, n, &result))
            return result;
    }
}
