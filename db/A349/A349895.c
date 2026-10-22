/*
 * A349895  Length of the longest self avoiding walk through a grid such that
 *          either x or y is changed by +1 or -1 in each step, and with 0 <=
 *          y, 0 <= x <= y, x + y <= n starting at (0,0) and terminating at
 *          (x,y) = (n,0).
 * https://oeis.org/A349895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   a(0) = 0 a(2k) = k * (k + 1) a(2k-1) = k² + k - 1 - 2 * floor((k
 *            + 1) / 4) where k = (n + 1) / 2 for odd n This implements the
 *            piecewise closed form given in OEIS A349895
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Even index: n = 2k */
    if ((n & 1ULL) == 0) {
        idx_t kidx = n >> 1;          /* k as idx_t */
        seq_t  k   = (seq_t)kidx;      /* convert to seq_t for arithmetic */
        return k * (k + 1);            /* k*(k+1) */
    }

    /* Odd index: n = 2k - 1  ->  k = (n + 1)/2 */
    idx_t kidx = (n + 1) >> 1;
    seq_t  k   = (seq_t)kidx;

    seq_t term = k * k + k - 1;          /* k² + k - 1 */
    seq_t sub  = 2 * ((k + 1) / 4);      /* 2*floor((k+1)/4) */
    return term - sub;                   /* result for odd n */
}
