/*
 * A278480  Number of neighbors of the n-th term in a full right triangle read
 *          by rows.
 * https://oeis.org/A278480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of Moore-neighbors (8-directional) inside the
 *            infinite upper‑left triangular array of points with rows r =
 *            1,2,… and columns c = 1…r. The n-th term is the n-th element in
 *            the triangle read row‑wise
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – value for n=0 is unused */

    /* Find the row r such that r*(r+1)/2 ≥ n using incremental summation */
    idx_t r = 0;        /* current row */
    idx_t tri = 0;      /* triangular number r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;        /* tri becomes r*(r+1)/2 */
    }

    /* Column c: number of elements before this row is tri - r */
    idx_t prev_tr = tri - r;   /* (r-1)*r/2 */
    idx_t c = n - prev_tr;     /* c ranges from 1 to r */

    /* Direction vectors for the 8 potential neighbors */
    const int dr[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    const int dc[8] = { -1,  0,  1,-1, 1,-1, 0, 1 };

    uint8_t count = 0;
    for (int i = 0; i < 8; ++i) {
        idx_t r2 = r + dr[i];
        idx_t c2 = c + dc[i];
        if (r2 >= 1 && c2 >= 1 && c2 <= r2) {
            ++count;
        }
    }
    return (seq_t)count;
}
