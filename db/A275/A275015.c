/*
 * A275015  Number of neighbors of each new term in an isosceles triangle read
 *          by rows.
 * https://oeis.org/A275015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*I(k>1)+I(k<=r-1) where r = minimal integer with
 *            r(r+1)/2 >= n (row index), k = n - r(r-1)/2 (position within the
 *            row)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, but guard for safety */

    /* Find the row r such that n belongs to row r. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low; /* row index */

    /* k is the position of n within its row (1‑based). */
    idx_t prevTri = (idx_t)(((__int128)(r - 1) * r) / 2);
    idx_t k = n - prevTri;

    /* Compute a(n) using the neighbor rule. */
    seq_t val = 0;
    if (k > 1)               /* left and upper‑left neighbors exist */
        val += 2;
    if (k <= r - 1)          /* upper‑right neighbor exists */
        val += 1;

    return val;
}
