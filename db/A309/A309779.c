/*
 * A309779  Squares that can be expressed as the sum of two positive squares
 *          but not as the sum of three positive squares.
 * https://oeis.org/A309779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25 * 4^(n-1) (n >= 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The OEIS offset is 1; for n < 1 we simply return 0. */
        return 0;
    }
    seq_t val = 25;          /* 5^2  */
    for (idx_t i = 1; i < n; ++i) {
        val <<= 2;           /* multiply by 4 each step */
    }
    return val;
}
