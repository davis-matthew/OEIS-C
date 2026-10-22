/*
 * A352422  Number of points available in a frame of snooker when there are n
 *          object balls remaining on the table.
 * https://oeis.org/A352422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = (n <= 6) ? (7*n - n*(n-1)/2) : (8*n - 21)
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if (n <= 6) {
        seq_t term = sn * 7 - (sn * (sn - 1)) / 2;
        return term;
    } else {
        seq_t term = sn * 8 - 21;
        return term;
    }
}
