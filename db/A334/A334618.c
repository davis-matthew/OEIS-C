/*
 * A334618  Irregular triangle read by rows: T(n,k), n >= 1, k >= 1, in which
 *          column k lists successive blocks of k consecutive integers that
 *          differ by 4, where the m-th block starts with m, m >= 1, and the
 *          first element of column k is in the row that is the k-th hexagonal
 *          number (A000384).
 * https://oeis.org/A334618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   hex_row_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A334618.
   The sequence is obtained by reading the irregular triangle T(n,k) by rows.
   Each column k contains successive blocks of k integers spaced by 4:
       block m (m ≥ 1) in column k = m, m+4, m+8, …, m+4(k‑1).
   The first element of column k appears in row H_k = k*(2*k‑1) (the k‑th
   hexagonal number).  For a given index n we locate the corresponding
   row and column and compute the value as
        T(n,k) = block + 4*offset,
   where block = ⌊(row‑H_k)/k⌋ + 1 and offset = (row-H_k) mod k. */

#include <stdint.h>
#include "../utils/hex_row_limit.h"

seq_t
a(idx_t n)
{
    idx_t count = 0;           /* number of terms before the current row   */
    for (idx_t row = 1; ; ++row)
    {
        idx_t kmax = hex_row_limit(row);
        if (count + kmax >= n)            /* the n‑th term is in this row   */
        {
            idx_t pos = n - count - 1;    /* position within the row (0‑based) */
            idx_t k   = pos + 1;          /* column index (1‑based)           */
            idx_t Hk  = k * (2 * k - 1);  /* first row of this column        */
            idx_t offset = row - Hk;      /* zero‑based index in the column  */
            idx_t block  = offset / k + 1;
            idx_t j      = offset % k;
            /* Cast only after computation to avoid overflow of intermediate
               64‑bit values.  seq_t may be larger than 64 bits in the
               arbitrary‑precision build. */
            return (seq_t)block + (seq_t)(4 * j);
        }
        count += kmax;                 /* move to the next row             */
    }
}
