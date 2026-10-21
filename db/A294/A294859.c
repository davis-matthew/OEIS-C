/*
 * A294859  Triangle whose n-th row is the concatenated sequence of all Lyndon
 *          compositions of n in lexicographic order.
 * https://oeis.org/A294859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_lyndon_vec, rec_lnds
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes a(n) by generating, for each integer, all Lyndon
 * compositions in lexicographic order.  The rows are concatenated
 * element‑wise and the nth element of this infinite sequence is returned.
 * For each row the recursive generator produces every composition of the
 * target weight; those that are Lyndon (primitive and lexicographically
 * smallest among their rotations) are counted.  The overall running time
 * is dominated by the enumeration of at most 2^(k-1) compositions for
 * the current row k (k≈20 for n≤10000), well below the 2 s limit.
 */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_lyndon_vec.h"
#include "../utils/rec_lnds.h"

/* Main sequence function.  n is 1‑based. */
seq_t a(idx_t n)
{
    idx_t rem = n;          /* remaining index to find */
    seq_t result = 0;       /* will hold the found value */

    for (idx_t row = 1; ; ++row) {
        int cur[128];          /* buffer for a composition (row ≤ 128) */
        bool found = false;
        rec_lnds(row, 0, cur, 0, &rem, &result, &found);
        if (found) return result;
        /* otherwise rem has been reduced by the entire row length
           and the loop continues with the next row.            */
    }
}
