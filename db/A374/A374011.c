/*
 * A374011  a(n) = (1 + (n+9)^2 - (n-10)*(-1)^n)/2.
 * https://oeis.org/A374011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (n+9)^2 - (n-10)*(-1)^n)/2
 * Range:     u8 n <= 7, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* t = n + 9,   sq = (n+9)^2 */
    seq_t t  = (seq_t)n + 9;
    seq_t sq = t * t;

    /* use the parity of n to avoid negative arithmetic on unsigned seq_t */
    seq_t val;
    if (n & 1U)          /* n is odd: (-1)^n = -1  →  val = sq + n - 9  */
        val = sq + (seq_t)n - 9;
    else                 /* n is even: (-1)^n = 1 →  val = sq - n + 11 */
        val = sq - (seq_t)n + 11;

    return val / 2;
}
