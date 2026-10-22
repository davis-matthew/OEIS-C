/*
 * A374933  Maximum number of squares covered (i.e., attacked) by 2
 *          independent (i.e., non-attacking) queens on an n X n chessboard.
 * https://oeis.org/A374933
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n - 20 for n >= 8; a(3)=9, a(4)=15, a(5)=23, a(6)=30,
 *            a(7)=37. Offset 3. Sequence A374933
 * Range:     u8 n <= 34, u16 n <= 8194, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) {
        /* Values below the offset are undefined; return 0. */
        return 0;
    }
    if (n <= 7) {
        static const seq_t tbl[5] = {9, 15, 23, 30, 37};
        return tbl[n - 3];
    }
    /* For n >= 8 the explicit formula applies. */
    return ((seq_t)n * 8) - 20;
}
