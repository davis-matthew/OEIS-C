/*
 * A378127  Inverse permutation to A377137.
 * https://oeis.org/A378127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by partitioning the positive integers into
 *            consecutive blocks (rows). For a row r len(r) = ((r&1) ? (r+1)/2
 *            : 3*r/2) // block length Let cum = sum_{i<r} len(i). The
 *            position of n inside row r is k = n - cum (1‑based). Set b =
 *            len(r). If (b - k) is even then p = (b - k + 2) / 2 ;...
 * Range:     u8 n <= 246, u16 n <= 9940, u32 n <= 9940, u64 n <= 9940,
 *            u128 n <= 9940, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t target = n;
    idx_t cumulative = 0;
    idx_t r = 0;

    while (1) {
        ++r;                       /* next row */
        idx_t len;
        if ((r & 1) == 0)          /* even row */
            len = 3 * (r / 2);
        else                       /* odd row */
            len = (r + 1) / 2;

        if (cumulative + len >= target) {
            idx_t k    = target - cumulative;          /* position in row, 1‑based */
            idx_t diff = len - k;                      /* b - k   */
            seq_t p;
            if (diff & 1)                               /* odd */
                p = ((seq_t)len + (seq_t)k + 1) / 2;
            else                                        /* even */
                p = ((seq_t)diff + 2) / 2;

            seq_t off = ((seq_t)r * (seq_t)r - ((r & 1) ? 1 : r)) / 2;
            return off + p;
        } else {
            cumulative += len;    /* keep going to next row */
        }
    }
}
