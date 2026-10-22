/*
 * A321358  a(n) = (2*4^n + 7)/3.
 * https://oeis.org/A321358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 7, a(0) = 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 3;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        val = 4 * val - 7;  /* recurrence: a(n) = 4*a(n-1) - 7 */
    }
    return val;
}
