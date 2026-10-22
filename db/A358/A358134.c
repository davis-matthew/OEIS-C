/*
 * A358134  Triangle read by rows whose n-th row lists the partial sums of the
 *          n-th composition in standard order (row n of A066099).
 * https://oeis.org/A358134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = partial sum of the ⌊n⌋‑th element of triangle A358134,
 *            constructed by listing, row by row, all compositions of m (m =
 *            1,2,…) in graded reverse‑lexicographic order, and outputting the
 *            cumulative sums of each composition
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   dfs_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Estimate the number of terms contributed by row m:
   L(1)=1, for m≥2: L(m) = (m+1)*2^(m-2)                                  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/dfs_row.h"

seq_t a(idx_t n)
{
    /* Find the row m that contains the n-th term. */
    idx_t cumulative = 0;                 /* terms seen so far */
    unsigned int m = 1;                   /* current sum value */

    while (true) {
        unsigned __int128 count;
        if (m == 1)
            count = 1;
        else
            count = ((unsigned __int128)(m + 1)) << (m - 2); /* (m+1)*2^(m-2) */

        if (cumulative + (idx_t)count >= n) {
            idx_t offset = n - cumulative - 1;   /* 0‑based offset in row */
            idx_t counter = 0;
            seq_t result = 0;
            bool found = false;
            unsigned int parts[64];
            dfs_row((int)m, 0, parts, offset, &counter, &result, &found);
            return result;
        }
        cumulative += (idx_t)count;
        ++m;
    }
}
