/*
 * A379759  Minimum number of kings that must be placed on an n X n chessboard
 *          such that each square is attacked or occupied by at least three
 *          kings.
 * https://oeis.org/A379759
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3k^2 + 2k if n = 3k * 3k^2 + 6k + 3 otherwise * where k =
 *            floor(n/3)
 * Range:     u8 n <= 26, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimum number of kings on an n×n board so that every square is
 * attacked or occupied by at least three kings.  This implements the
 * explicit piecewise formula derived from the OEIS data. */
seq_t a(idx_t n)
{
    /* n is at least 2 as per the OEIS offset */
    idx_t k   = n / 3;          /* floor(n/3) */
    idx_t rem = n % 3;          /* n mod 3 */

    if (rem == 0) {
        /* a(3k) = 3k^2 + 2k */
        return (seq_t)3 * (seq_t)k * (seq_t)k  +  (seq_t)2 * (seq_t)k;
    } else {
        /* a(3k+1) = a(3k+2) = 3k^2 + 6k + 3 */
        return (seq_t)3 * (seq_t)k * (seq_t)k  +  (seq_t)6 * (seq_t)k + (seq_t)3;
    }
}
