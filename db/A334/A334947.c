/*
 * A334947  Irregular triangle read by rows: T(n,k) is the number of parts in
 *          the partition of n into k consecutive parts that differ by 6, n >=
 *          1, k >= 1, and the first element of column k is in the row that is
 *          the k-th octagonal number (A000567).
 * https://oeis.org/A334947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   max_k_for_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A334947 (irregular triangle read by rows).
   The series is generated from the triangle T(n,k) defined as: 
   T(n,k) = k if n is divisible by k and n >= k*(3k-2), otherwise 0.
   The sequence a(n) is obtained by flattening the triangle by rows
   (row-major order), i.e. a = T(1,1), T(2,1), T(3,1), ..., T(8,1), T(8,2),
   T(9,1), T(9,2), ... .   */

#include <stdint.h>
#include <stddef.h>
#include "../utils/max_k_for_row.h"

/* Return the value of the triangle entry (row, col).  */
static inline seq_t triangle_value(idx_t row, idx_t col)
{
    /* By definition, T(row,col) = col if row % col == 0, otherwise 0.  */
    return (row % col == 0) ? (seq_t)col : (seq_t)0;
}

/* Main function required by the build.   */
seq_t a(idx_t n)
{
    /* n is 1‑based.  We find the row r such that the running total of
       columns up to r-1 is less than n, and the running total up to r
       is not less than n.  Then the column is n minus the total
       before row r.  This allows us to handle any order of calls.  */
    idx_t remaining = n;   /* number of entries still to skip   */
    idx_t row = 1;         /* current row in the triangle       */

    while (1) {
        idx_t kmax = max_k_for_row(row);   /* columns in this row      */
        if (remaining <= kmax) {
            idx_t col = remaining;          /* 1‑based column index   */
            return triangle_value(row, col);
        }
        remaining -= kmax;
        ++row;
    }
}
