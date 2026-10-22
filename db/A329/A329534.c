/*
 * A329534  Irregular triangle read by rows: for n >= 1 row n lists the k from
 *          [1, 2, ... , n] such that A002378(k-1) = (k-1)*k == 0 (mod n).
 * https://oeis.org/A329534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the nth element of the irregular triangle read by rows.
 *            Row r lists integers k (1 ≤ k ≤ r) such that r divides k·(k‑1)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, safe fallback */

    idx_t cumulative = 0;          /* number of elements seen so far */
    for (idx_t r = 1; ; ++r) {     /* iterate over rows until the nth element is reached */
        idx_t len = 0;             /* number of solutions in current row */

        /* count solutions in row r */
        for (idx_t k = 1; k <= r; ++k) {
            unsigned long long prod = (unsigned long long)k * (k - 1);
            if (prod % r == 0) ++len;
        }

        if (cumulative + len >= n) {
            /* the answer lies in this row */
            idx_t target = n - cumulative;   /* 1‑based position inside row r */

            idx_t count = 0;
            for (idx_t k = 1; k <= r; ++k) {
                unsigned long long prod = (unsigned long long)k * (k - 1);
                if (prod % r == 0) {
                    ++count;
                    if (count == target)
                        return (seq_t)k;     /* found the required element */
                }
            }
            /* control never reaches here, but keep compiler happy */
            return 0;
        }

        cumulative += len;           /* move to next row */
    }
}
