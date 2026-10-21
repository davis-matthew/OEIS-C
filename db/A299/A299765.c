/*
 * A299765  Irregular triangle read by rows, T(n,k), n >= 1, k >= 1, in which
 *          row n lists the partitions of n into consecutive parts, with the
 *          partitions ordered by increasing number of parts.
 * https://oeis.org/A299765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(n) = the n‑th part of the triangle of partitions of integers *
 *            into consecutive parts, read by rows and within each row by *
 *            increasing number of parts. Each partition is written in
 *            decreasing * order and the parts of all partitions are
 *            concatenated row by row. * * This implementation follows t...
 * Range:     u8 n <= 3763, u16 n <= 9987, u32 n <= 9987, u64 n <= 9987,
 *            u128 n <= 9987, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* we assume n ≥ 1 */
    idx_t cumulative = 0;   /* total number of terms seen so far */
    idx_t row = 0;          /* current row (ρ) */

    while (true) {
        row++;

        /* First compute how many terms the current row contributes. */
        idx_t row_terms = 0;
        for (idx_t k = 1;; ++k) {
            idx_t tri = k * (k - 1) / 2;
            if (tri >= row)
                break;
            idx_t t = row - tri;
            if (t % k == 0)              /* valid partition with k parts */
                row_terms += k;          /* each of its k parts is output */
        }

        /* Does the requested index lie inside this row? */
        if (cumulative + row_terms >= n) {
            idx_t pos_in_row = n - cumulative;   /* 1‑based within the row */

            /* Find the exact partition and part. */
            for (idx_t k = 1;; ++k) {
                idx_t tri = k * (k - 1) / 2;
                if (tri >= row)
                    break;
                idx_t t = row - tri;
                if (t % k == 0) {
                    idx_t a_start = t / k;          /* smallest part */
                    /* Partition parts in decreasing order: a_start+k-1 … a_start */
                    if (pos_in_row <= k) {
                        idx_t part = (a_start + k - 1) - (pos_in_row - 1);
                        return (seq_t)part;
                    } else {
                        pos_in_row -= k;
                    }
                }
            }
            /* Should never reach here */
            return 0;
        } else {
            cumulative += row_terms;   /* move to next row */
        }
    }
}
