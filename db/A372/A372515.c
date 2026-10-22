/*
 * A372515  Irregular triangle read by rows where row n lists the positions of
 *          zeros in the reversed binary expansion of n.
 * https://oeis.org/A372515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A372515: Irregular triangle read by rows where row n lists
 * the positions of zeros in the reversed binary expansion of n.
 * a(n) is the n-th entry in this triangle (entries are read row‑by‑row).
 * The algorithm walks over rows, accumulating their zero‑counts until the
 * target index is located, then returns the appropriate zero position. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    idx_t cum = 0;          /* cumulative number of entries seen so far */
    idx_t row = 1;          /* current row being processed */

    while (1) {
        /* Count zeros in the reversed binary representation of 'row'. */
        idx_t tmp = row;
        idx_t zeros = 0;
        while (tmp > 0) {
            if ((tmp & 1ULL) == 0) zeros++;   /* bit is '0' */
            tmp >>= 1;
        }

        /* If the target position lies before this row, skip it. */
        if (cum + zeros < n) {
            cum += zeros;
            ++row;
            continue;
        }

        /* The desired entry is in this row.  Determine which zero. */
        idx_t k = n - cum;          /* 1‑based index within the row */

        idx_t pos = 1;              /* current bit position (LSB = 1) */
        tmp = row;
        while (tmp > 0) {
            if ((tmp & 1ULL) == 0) { /* found a zero in this position */
                if (--k == 0)
                    return (seq_t)pos;     /* return the zero's position */
            }
            tmp >>= 1;
            ++pos;
        }

        /* If we get here something went wrong (should not happen). */
        return (seq_t)0;
    }
}
