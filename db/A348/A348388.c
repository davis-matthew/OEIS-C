/*
 * A348388  Irregular triangle read by rows: T(n, k) = floor((n-k)/k), for k =
 *          1, 2, ..., floor(n/2) and n >= 2.
 * https://oeis.org/A348388
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9326, u16 n <= 9326, u32 n <= 9326, u64 n <= 9326,
 *            u128 n <= 9326, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A348388
 *  Irregular triangle read by rows (starting at offset 2).
 *  Triangle:   T(n,k) = floor((n-k)/k),   1 ≤ k ≤ floor(n/2),  n ≥ 2.
 *  Sequence a(n) is the list of all T(n,k) concatenated row‑by‑row.
 *
 *  For a given index n (n ≥ 2) the function returns the corresponding
 *  triangle entry.
 *
 *  The cumulative number of elements up to row m is:
 *      cumLen(m) = { (m/2)^2            if m even
 *                  { (⌊m/2⌋)*(⌊m/2⌋+1) if m odd   }
 *
 *  Algorithm:
 *      1. Convert the 1‑based index n to 0‑based position i = n-2.
 *      2. Find the smallest m such that cumLen(m) > i.
 *      3. In row m the element number is k = (i - cumLen(m-1)) + 1.
 *      4. Return T(m,k) = (m-k)/k  (integer division).
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    if (n < 2) {           /* undefined for n < 2, return 0 as safe */
        return (seq_t)0;
    }

    /* zero‑based position in the flattened sequence */
    idx_t i = n - 2;

    /* locate the row containing position i */
    idx_t m = 2;               /* start from the first row (n = 2) */
    idx_t cumulative = 0;      /* total elements up to previous row */

    while (1) {
        /* length of current row: floor(m/2) */
        idx_t row_len = m >> 1;   /* m/2 */

        if (cumulative + row_len > i) {
            break;              /* i lies in this row */
        }

        cumulative += row_len;
        ++m;
    }

    /* position within row m (0‑based) */
    idx_t pos_in_row = i - cumulative;
    idx_t k = pos_in_row + 1;    /* k in 1..floor(m/2) */

    /* compute T(m,k) = floor((m-k)/k) */
    seq_t numerator   = (seq_t)(m - k);   /* m and k fit into seq_t */
    seq_t denominator = (seq_t)k;
    return numerator / denominator;
}
