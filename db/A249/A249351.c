/*
 * A249351  Triangle read by rows in which row n lists the widths of the
 *          symmetric representation of sigma(n).
 * https://oeis.org/A249351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   _floor_isqrt_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A249351: widths of the symmetric representation of σ(n).
   For a flattened index n (starting at 1), this function returns the
   corresponding entry of the triangle read by rows.  Each row n has
   length 2n‑1, is symmetric, and its elements sum to σ(n).  If σ(n)
   exceeds the row's length, the centre element is increased accordingly;
   otherwise zeros are inserted as close to the centre as possible
   following the rules extracted from the OEIS definition. */

#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Determine the row (1‑based) and position within that row. */
    idx_t r = _floor_isqrt_u64(n - 1) + 1;          /* row number */
    idx_t prev_sq = (r - 1) * (r - 1);              /* elements before row r */
    idx_t pos = n - prev_sq;                        /* 1‑based position in row */

    /* Row properties */
    idx_t len = 2 * r - 1;                          /* length of the row */
    seq_t sigma = sigma_of(r);                      /* σ(r) */

    if (sigma <= (seq_t)len) {
        /* Reduce sum by inserting zeros.  The number of zeros is
           zero_count = len – σ(r).  Zero placement rules: */
        idx_t zero_count = (idx_t)((seq_t)len - sigma);
        if (zero_count == 0) return (seq_t)1;       /* all ones */

        /* Centre index (0‑based). */
        idx_t center0 = r - 1;
        idx_t pos0 = pos - 1;
        idx_t dist = (pos0 > center0) ? (pos0 - center0) : (center0 - pos0);

        if (zero_count & 1) {                      /* odd number of zeros */
            /* Centre is zero; remaining zeros form a contiguous block. */
            idx_t half = zero_count >> 1;           /* floor(zero_count/2) */
            if (dist <= half) return (seq_t)0;
        } else {                                    /* even number of zeros */
            /* Zeros occupy symmetric pairs at distances 2 … half+1. */
            idx_t half_pairs = zero_count >> 1;     /* number of symmetric pairs */
            idx_t lower = 2;
            idx_t upper = half_pairs + 1;
            if (dist >= lower && dist <= upper) return (seq_t)0;
        }
        return (seq_t)1;
    } else {
        /* σ(r) > len: increase the centre element. */
        seq_t inc = sigma - (seq_t)len;
        idx_t center0 = r - 1;
        idx_t pos0 = pos - 1;
        if (pos0 == center0) return 1 + inc;       /* centre element */
        else return (seq_t)1;                      /* other elements remain 1 */
    }
}
