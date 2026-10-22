/*
 * A389733  Irregular triangle read by rows where row n is the 0-prepended
 *          first differences of the n-th composition in standard order.
 * https://oeis.org/A389733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   OEIS A389733
 *   Irregular triangle read by rows.  Row n contains the 0‑prefixed
 *   first differences of the n‑th composition in standard order
 *   (graded reverse‑lexicographic, A066099).  The sequence a(n) is
 *   obtained by concatenating the rows; a(1) is the first term.
 *
 *   Algorithm:
 *   For each integer k = 1,2,3,… generate its composition:
 *     1. Find positions of 1's in the reversed binary expansion of k
 *        (positions counted from 1).
 *     2. Prepend 0 to this set.
 *     3. Take first differences of the resulting list.
 *     4. Reverse the list – that gives the parts of the composition.
 *   For the obtained parts p[0..m-1], compute the row values:
 *     row[0]   = p[0];
 *     row[i]   = p[i] - p[i-1]   (i ≥ 1).
 *   Concatenate rows until the cumulative count of elements reaches n,
 *   then return the appropriate element.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t cum = 0;         /* number of elements produced so far */

    for (idx_t k = 1;; ++k) {
        /* --- step 1: collect positions of 1's in reversed binary of k --- */
        idx_t pos[64];
        size_t mpos = 0;
        idx_t tk = k;
        for (size_t i = 0; tk; ++i) {
            if (tk & 1) {
                pos[mpos++] = (idx_t)(i + 1);     /* positions are 1‑based */
            }
            tk >>= 1;
        }

        /* --- step 2 & 3: prepend 0 and take first differences --- */
        idx_t arr[65];          /* arr[0] = 0, arr[1..mpos] = pos[0..mpos-1] */
        arr[0] = (idx_t)0;
        for (size_t j = 0; j < mpos; ++j) {
            arr[j + 1] = pos[j];
        }
        size_t m = mpos;        /* number of parts in the composition */

        /* parts_forward[j] = arr[j+1] - arr[j]  for j = 0..m-1   */
        idx_t parts_forward[64];
        for (size_t j = 0; j < m; ++j) {
            parts_forward[j] = arr[j + 1] - arr[j];
        }

        /* reverse to obtain the composition parts */
        seq_t parts[64];
        for (size_t j = 0; j < m; ++j) {
            parts[j] = (seq_t)parts_forward[m - 1 - j];
        }

        /* --- compute row values: 0‑prefixed first differences of parts --- */
        seq_t row[64];
        if (m > 0) {
            row[0] = parts[0];
            for (size_t j = 1; j < m; ++j) {
                row[j] = parts[j] - parts[j - 1];
            }
        } else {
            continue;   /* k = 0 would produce an empty composition, but k≥1 here */
        }

        /* --- check if the target index n falls in this row --- */
        if (cum + m >= n) {
            idx_t offset = n - cum - 1;        /* zero‑based within the row */
            return row[offset];
        }
        cum += m;
    }
}
