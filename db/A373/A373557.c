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
 * Validated: standard, pass, 105 terms through n = 9690 (2026-11-01)
 * Helpers:   none
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

static int
rec_gen(uint64_t m, uint64_t max_val, int depth,
        uint64_t *cur, idx_t *cnt, idx_t n, seq_t *ans)
{
    for (uint64_t i = 2; i <= max_val; ++i) {
        cur[depth] = i;
        uint64_t sz = depth + 1;              /* |X| after adding i */
        if (sz + 1 < i) {                     /* size < min(i)  */
            ++(*cnt);
            if (*cnt == n) { *ans = (seq_t)m; return 1; }
            /* Output elements of X in decreasing order.
             *   cur is stored in ascending order (smallest to largest),
             *   so we output from cur[0] up to cur[depth] to
             *   produce decreasing order. */
            for (int j = 0; j <= depth; ++j) {
                ++(*cnt);
                if (*cnt == n) { *ans = (seq_t)cur[j]; return 1; }
            }
        }
        if (rec_gen(m, i - 1, depth + 1, cur, cnt, n, ans))
            return 1;
    }
    return 0;
}

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
