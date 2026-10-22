/*
 * A343854  Irregular triangle read by rows: the n-th row gives the column
 *          indices of the matrix of 1..n^2 filled successively back and forth
 *          along antidiagonals.
 * https://oeis.org/A343854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) = column index (1‑based) of the element numbered n in the
 *            irregular triangle where the matrix of size r×r is filled with
 *            1..r² along antidiagonals, alternating direction each diagonal
 *            (even diagonals: bottom‑to‑top, odd diagonals: top‑to‑bottom)
 * Range:     u8 n <= 9455, u16 n <= 9455, u32 n <= 9455, u64 n <= 9455,
 *            u128 n <= 9455, big any n
 * Helpers:   sumsq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sumsq.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;      /* undefined for n < 1, but safe guard */
    if (n == 1) return (seq_t)1;     /* a(1) = 1 */

    /* find the row r such that sumsq(r‑1) < n ≤ sumsq(r) */
    idx_t low = 1, high = 1;
    while (sumsq(high) < n) high <<= 1;          /* exponential search upper bound */
    while (low < high) {                         /* binary search */
        idx_t mid = low + (high - low) / 2;
        if (sumsq(mid) >= n) high = mid;
        else                  low = mid + 1;
    }
    idx_t r = low;                               /* current row number */

    /* offset of the element inside this row (0‑based) */
    idx_t prevSum = sumsq(r - 1);
    idx_t offset  = n - prevSum - 1;

    /* locate the diagonal containing the element */
    idx_t cumulative = 0;
    idx_t maxS = 2 * r - 2;                      /* last diagonal index (0‑based) */
    for (idx_t s = 0; s <= maxS; ++s) {
        idx_t len = (s < r) ? (s + 1) : (2 * r - 1 - s);  /* length of diagonal s */
        if (cumulative + len > offset) {            /* element lies on diagonal s */
            idx_t k = offset - cumulative;          /* position within diagonal */

            idx_t i_low  = (s >= r) ? (s - (r - 1)) : 0;
            idx_t i_high = (s < r)  ?  s           : (r - 1);
            seq_t j;                                /* column index (0‑based) */

            if ((s & 1) == 0) {                    /* even s : i decreases, j increases */
                idx_t offset0 = s - i_high;
                j = offset0 + k;
            } else {                               /* odd s : i increases, j decreases */
                idx_t offset1 = s - i_low;
                j = offset1 - k;
            }
            return (seq_t)(j + 1);                 /* convert to 1‑based column index */
        }
        cumulative += len;
    }
    return (seq_t)0;  /* unreachable */
}
