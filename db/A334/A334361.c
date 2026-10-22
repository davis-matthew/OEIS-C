/*
 * A334361  Number of r X s rectangles with squarefree side lengths such that
 *          r < s, r + s = 2n and r | s.
 * https://oeis.org/A334361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integers r in [1..n‑1] such that r | (2n‑r) and
 *            both r and (2n‑r) are squarefree
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t two_n = (seq_t)n * 2;
    for (idx_t i = 1; i < n; ++i) {
        seq_t j = two_n - (seq_t)i;
        if (j % (seq_t)i != 0) continue;          /* r does not divide s */
        if (!is_squarefree((seq_t)i)) continue;    /* r not squarefree   */
        if (!is_squarefree(j)) continue;           /* s not squarefree   */
        ++count;
    }
    return count;
}
