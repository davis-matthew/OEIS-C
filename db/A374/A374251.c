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
 * Validated: partial, pass, 84 terms through n = 84 (2026-11-01)
 * Helpers:   none
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

static int dfs_comp(idx_t sum, int *parts, idx_t depth, idx_t *cnt,
                    idx_t target, seq_t *res)
{
    if (sum == 0) {                 /* finished one composition */
        int prev = -1;
        for (idx_t i = 0; i < depth; ++i) {
            int p = parts[i];
            if (prev == -1 || p != prev) { /* new run part */
                ++(*cnt);
                if (*cnt == target) {
                    *res = (seq_t)p;
                    return 1;
                }
                prev = p;
            }
        }
        return 0;
    }

    for (idx_t p = sum; p >= 1; --p) {
        parts[depth] = (int)p;
        if (dfs_comp(sum - p, parts, depth + 1, cnt, target, res))
            return 1;
    }
    return 0;
}

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
