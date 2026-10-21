/*
 * A279211  Fill an array by antidiagonals upwards; in the n-th cell, enter
 *          the number of earlier cells that can be seen from that cell.
 * https://oeis.org/A279211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = x + 2*y + min(x, y) where (x, y) are the coordinates of
 *            the n‑th cell in antidiagonal order (x + y = s) with x
 *            decreasing inside each antidiagonal
 * Range:     u8 n <= 5049, u16 n <= 5049, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the antidiagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t low = 0;
    idx_t high = 1;
    /* exponential search for an upper bound */
    while ( (((__int128)high) * (high + 1)) / 2 <= n )
        high <<= 1;

    /* binary search on [low, high] */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        __int128 t = (((__int128)mid) * (mid + 1)) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t s = low;

    /* Offset within the antidiagonal: y = n - s*(s+1)/2 */
    idx_t y = n - ( ( (__int128)s * (s + 1) ) / 2 );
    /* Coordinates: x = s - y */
    idx_t x = s - y;

    /* Compute a(n) = x + 2*y + min(x, y) */
    if (x >= y) {
        /* a = x + 3*y */
        return (seq_t)x + (seq_t)3 * (seq_t)y;
    } else {
        /* a = 2*(x + y) */
        return (seq_t)2 * ((seq_t)x + (seq_t)y);
    }
}
