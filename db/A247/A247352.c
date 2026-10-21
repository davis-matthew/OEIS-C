/*
 * A247352  Rectangular array read by upwards by columns: T = T(n,k) = number
 *          of paths from (0,1) to (n,k), where 0 <= k <= 3, consisting of
 *          segments given by the vectors (1,1), (1,2), (1,-1).
 * https://oeis.org/A247352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = number of paths from (0,1) to (⌊n/4⌋, n mod 4) using
 *            steps (1,1),(1,2),(1,-1). The table T(m,k) satisfies T(0,0)=0,
 *            T(0,1)=1, T(0,2)=0, T(0,3)=0, T(m,0)=T(m-1,1),
 *            T(m,1)=T(m-1,0)+T(m-1,2), T(m,2)=T(m-1,1)+T(m-1,0)+T(m-1,3),
 *            T(m,3)=T(m-1,2)+T(m-1,1). Then a(4m)=T(m,0), a(4m+1)=T(m,1),
 *            a(4m+...
 * Range:     u8 n <= 40, u16 n <= 73, u32 n <= 141, u64 n <= 273,
 *            u128 n <= 541, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Row (i.e. m) and column (k) within the table. */
    idx_t m = n / 4;
    idx_t k = n % 4;

    /* Base case: row 0.  Stored explicitly. */
    seq_t row0[4] = { (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)0 };
    if (m == 0)
        return row0[k];

    /* Temporary arrays for the current and previous rows. */
    seq_t prev[4];
    seq_t cur[4];
    prev[0] = row0[0];
    prev[1] = row0[1];
    prev[2] = row0[2];
    prev[3] = row0[3];

    /* Compute rows up to m. */
    for (idx_t row = 1; row <= m; ++row) {
        cur[0] = prev[1];
        cur[1] = prev[0] + prev[2];
        cur[2] = prev[1] + prev[0] + prev[3];
        cur[3] = prev[2] + prev[1];

        /* Copy current row into prev for next iteration. */
        prev[0] = cur[0];
        prev[1] = cur[1];
        prev[2] = cur[2];
        prev[3] = cur[3];
    }

    /* Return the requested entry. */
    return prev[k];
}
