/*
 * A271344  Triangle read by rows: T(n,k), n>=1, k>=1, in which column k lists
 *          the odd numbers multiplied by -1, interleaved with k-1 zeros, but
 *          T(n,1) = 1 and the first element of column k is in row k(k+1)/2.
 * https://oeis.org/A271344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) is obtained from the triangle T(n,k): T(n,1) = 1; For k ≥ 2
 *            let r₀ = k(k+1)/2. If n < r₀ then T(n,k) = 0. Else if (n–r₀) mod
 *            k == 0 then M = (n–r₀)/k T(n,k) = –(2·M + 1); else T(n,k) = 0.
 *            The sequence a(n) is the row‑major flattening of T
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Helpers:   column_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/column_value.h"

static inline idx_t row_length(idx_t r)
{
    idx_t k = 0;
    while (((k + 1) * (k + 2)) / 2 <= r)
        ++k;
    return k;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* outside defined range */
    idx_t cum = 0;
    idx_t r;
    for (r = 1; ; ++r)
    {
        idx_t len = row_length(r);
        if (cum + len >= n)
            break;
        cum += len;
    }
    idx_t pos = n - cum - 1;      /* zero‑based column index within the row */
    idx_t col = pos + 1;           /* columns start at 1 */
    return column_value(r, col);
}
