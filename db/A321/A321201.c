/*
 * A321201  Irregular triangle T with the nontrivial solutions of 2*e2 + 3*e3
 *          = n, for n >= 2, with nonnegative e2 and e3, ordered as pairs with
 *          increasing e2 values.
 * https://oeis.org/A321201
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth element of the flattened triangle of pairs (e2,
 *            e3) that satisfy 2*e2 + 3*e3 = n for n ≥ 2, where the pairs are
 *            ordered by increasing e2. The sequence is formed by writing each
 *            pair as [e2, e3] and then reading the e2 and e3 values
 *            alternately
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   num_solutions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/num_solutions.h"

/* Return the nth element of the flattened list.  n is 1‑based with
   the OEIS offset of 2, i.e. a(2) is the first element. */
seq_t a(idx_t n) {
    if (n < 2) /* behaviour below the offset is undefined */
        return 0;

    /* Convert to zero‑based position in the flattened sequence.  */
    idx_t pos = n - 2;

    idx_t cumulative = 0;      /* number of elements seen so far */
    for (idx_t row = 2; ; ++row) {
        idx_t k = num_solutions(row);   /* number of pairs in this row   */
        idx_t row_len = k << 1;          /* two numbers per pair          */
        if (cumulative + row_len > pos) {
            /* The desired element lies in this row.  */
            idx_t offset_in_row = pos - cumulative;      /* 0‑based   */
            idx_t pair_index    = offset_in_row >> 1;     /* integer div by 2 */
            idx_t inside_pair   = offset_in_row & 1;      /* 0 → e2, 1 → e3 */

            /* Determine the e3 value for the pair_index‑th pair
               (pairs are ordered by increasing e2, i.e. decreasing e3). */
            idx_t max_e3 = row / 3;
            if ((max_e3 & 1) != (row & 1))
                max_e3--;
            idx_t e3 = max_e3 - (pair_index << 1);
            idx_t e2 = (row - (e3 * 3)) >> 1;

            return (inside_pair == 0) ? (seq_t)e2 : (seq_t)e3;
        }
        cumulative += row_len;
    }
}
